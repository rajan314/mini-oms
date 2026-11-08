
# Week 1 — Trading Basics Notes
 
Date: 8 Nov 2026
Topics: orders · bid/ask/spread/tick · order book · matching (price-time priority) · order lifecycle · OMS vs EMS vs exchange
 
---
 
## 1. What an order is
 
An order = an instruction to buy or sell. These fields become my C++ `struct Order` later.
 
| Field | Example | Meaning |
| --- | --- | --- |
| Symbol | RELIANCE | What to trade |
| Side | Buy / Sell | Direction |
| Quantity | 100 | How many shares |
| Price | 2500.50 | Worst price I accept (limit orders only) |
| Order type | Limit / Market | How price is handled |
| Order ID | 1001 | Unique id to track it |
 
### Order types
- **Limit order**: "Buy 100 at 2500 or lower." Never trades at a worse price. If nothing matches, it WAITS in the book.
- **Market order**: "Buy 100 now at any price." Trades immediately, can get a bad price, NEVER waits in the book.
## 2. Bid, ask, spread, tick
 
- **Bid** = highest price a buyer will pay right now.
- **Ask / Offer** = lowest price a seller will accept right now.
- **Spread** = ask − bid. Small spread = liquid (easy to trade).
- **Tick size** = smallest allowed price step. NSE: ₹0.05 for most stocks.
  - Valid: 2500.00, 2500.05, 2500.10 … Invalid: 2500.03 → order REJECTED.
  - That's why prices are stored as integer ticks in code (never `double`).
## 3. The order book
 
List of all waiting limit orders.
 
```
        BIDS (buyers)          |      ASKS (sellers)
   Qty    Price                |   Price     Qty
   200    2500.00  ← best bid  |   2500.50   150  ← best ask
   500    2499.50              |   2501.00   300
   100    2499.00              |   2502.00   400
```
 
- Bids sorted HIGHEST first. Asks sorted LOWEST first.
- A trade happens only when buy price ≥ sell price. Otherwise everyone waits.
## 4. Matching rules
 
1. **Buys trade against ASKS. Sells trade against BIDS.** (Never the same side.)
2. **Price priority**: best price first (highest bid, lowest ask).
3. **Time priority**: same price → whoever came first goes first (FIFO queue).
   - Together = **price-time priority**. Used by NSE, BSE and most exchanges.
4. **Trade price = the RESTING (waiting) order's price.** The incoming order gets the better deal.
   - Buy @ 2502 meets resting sell @ 2500.50 → trade at 2500.50.
5. **You never get more than you asked for.** Leftover of the resting order stays in the book (partial fill).
6. **Walking the book**: an order that is bigger than the best level keeps trading at the next levels, at worse prices.
7. **Modify rules**:
   - Change price or INCREASE qty → lose place, go to back of queue.
   - REDUCE qty → keep place.
   - A better price beats an earlier arrival.
### Code link
Each price level is a queue: new orders join the back, matching takes from the front → `std::list<Order>` per price level.
 
## 5. Order lifecycle (state machine — coded in Week 10)
 
| State | Meaning | Final? |
| --- | --- | --- |
| Pending New | Sent, exchange hasn't confirmed | No |
| New | Accepted, waiting in book | No |
| Partially Filled | Some traded, rest waiting | No |
| Filled | All traded | **Yes** |
| Cancelled | Removed before fully filled | **Yes** |
| Rejected | Refused (bad price, risk check failed…) | **Yes** |
 
```
             ┌──→ Rejected
Pending New ─┴──→ New ──→ Partially Filled ──→ Filled
                   │            │  ↺ more fills
                   ↓            ↓
                Cancelled    Cancelled
```
 
Rules:
- ANY trade on a New order → Partially Filled. Leaves = 0 → Filled.
- **Final states never change.** Filled → Cancelled is an illegal transition; code must refuse it.
- Cancel only removes the LEAVES quantity. Traded part stays traded.
- Leaves = 0 → nothing to cancel → **Cancel Reject** ("too late to cancel"), order stays Filled.
- **Pending Cancel**: a cancel is not instant. A fill can arrive before the cancel → final state can be Filled. (Classic interview question.)
### Two numbers on every order
- **Filled qty (cum qty)** = traded so far
- **Leaves qty** = still waiting
- **Check: filled + leaves = original qty, always.**
Example: Sell 200, trades 50, 100, 50
 
| Event | State | Filled | Leaves |
| --- | --- | --- | --- |
| Accepted | New | 0 | 200 |
| Trade 50 | Partially Filled | 50 | 150 |
| Trade 100 | Partially Filled | 150 | 50 |
| Trade 50 | Filled | 200 | 0 |
 
- Invalid order (e.g. price 2500.03): Pending New → Rejected. Never reaches the book. (Like HTTP 400 Bad Request.)
- **Execution Report** = exchange's message telling the OMS about every change (FIX 35=8). Built in Project 3.
## 6. OMS vs EMS vs Exchange
 
```
Trader / Portfolio Manager
        ↓
       OMS   → what to trade, are we allowed? (record, positions, P&L, compliance, risk)
        ↓
       EMS   → how to trade it best? (algos like TWAP/VWAP, smart order routing, market data)
        ↓
 Broker → EXCHANGE (NSE/BSE) → matching engine, market data
        ↓
 Clearing house → settlement (T+1 in India)
 
 Execution reports flow back up the same path.
```
 
| System | Job | Full stack analogy |
| --- | --- | --- |
| OMS | Official record, rules, state, pre-trade checks | Backend business logic + database of record |
| EMS | Executes well: slicing, routing | Smart API gateway |
| Exchange | Order book + matching | External service; execution reports = webhooks |
| Clearing | Moves shares and money | — |
 
- **OEMS** = OMS + EMS in one product (common today).
- Risk checks at many layers: OMS → broker RMS (margin) → exchange.
- Examples: Charles River, Bloomberg AIM (buy-side OMS); Zerodha runs its own OMS + RMS behind Kite.
### Key points
- **Exchange is the source of truth.** If OMS says Partially Filled and exchange says Filled, the OMS is just behind — the execution report hasn't arrived yet.
  - Lost messages → on reconnect, FIX **sequence numbers** + resend (Week 20).
  - End of day → **reconciliation** with exchange/broker records.
- **Don't send huge market orders**: they walk the book → **market impact** (you push price against yourself) and **slippage** (expected price vs average paid). Everyone sees a big buyer. → EMS slices it (TWAP, Project 5).
- **Pre-trade limits are checked first in the OMS** (cheapest, safest place to stop a bad order).
### My project = a miniature of this
| Project | Role |
| --- | --- |
| 1: Order book + matching engine | Exchange |
| 2: OMS (state machine, risk, P&L) | OMS |
| 3: FIX over TCP + web dashboard | Connection + trader's screen |
| 5 (stretch): TWAP + smart router | EMS |
 
## 7. Glossary
 
| Term | Meaning |
| --- | --- |
| Liquid | Easy to trade, small spread, lots of quantity |
| Resting order | Order waiting in the book |
| Aggressive / incoming order | New order that trades immediately against resting ones |
| Partial fill | Only part of the quantity traded |
| Price level | All orders at one price |
| FIFO | First in, first out |
| Walking the book | Trading through several price levels |
| Market impact | Your own order moving the price against you |
| Slippage | Expected price vs actual average price |
| Execution Report | Message reporting fills/state changes (FIX 35=8) |
| Cancel Reject | "Too late to cancel" reply |
| RMS | Risk Management System (broker's margin checks) |
| T+1 | Settlement one day after the trade |
 
## 8. My mistakes this week (learn from them)
 
1. Buy 100 vs resting 150 → I said 150 traded. **Wrong: only 100. Leftover 50 stays.**
2. Market buy → I looked at the bid side. **Wrong: buys always trade against ASKS.**
3. Invalid price 2500.03 → I said "no sellers". **Wrong: tick size 0.05 → Rejected before reaching the book.**
4. After the first fill I kept state = New. **Wrong: any fill → Partially Filled.**
5. Forgot the final fill → Filled with leaves 0. **Check filled + leaves = original.**
6. Cancel on a Filled order → I described partial cancel. **Wrong: nothing left → Cancel Reject, stays Filled.**
7. Big market order → I said "because of state". **Wrong: market impact / walking the book.**
8. OMS vs exchange mismatch → I trusted the OMS. **Wrong: exchange is the truth; OMS catches up via execution reports.**
✓ Got right: cancel race (fill arrives before cancel → Filled), pre-trade limit stopped by OMS.
 
## 9. Practice for the rest of the week
 
- [ ] Tue: draw the lifecycle state diagram from memory
- [ ] Thu: watch a video on "how an exchange matching engine works"
- [ ] Weekend: write my own NOTES.md, compare with this file
- [ ] Re-solve the Topic 2 exercise without looking at the answers