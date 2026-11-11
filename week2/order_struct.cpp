#include <cstdint>
#include <iostream>
#include <string>

enum class Side
{
  Buy,
  Sell
};

struct Order
{
  std::uint64_t id;
  Side side;
  std::int64_t price; // in ticks: 100.50 ->10050
  std::uint32_t qty;

  bool isBuy() const
  {
    return side == Side::Buy;
  }
};

std::string toString(Side side)
{
  return side == Side::Buy ? "BUY" : "SELL";
}

void printOrder(const Order &o)
{
  std::cout << "id=" << o.id << " " << toString(o.side) << " price=" << o.price << "qty=" << o.qty << '\n';
}
int main()
{
  const Order buy{1, Side::Buy, 10050, 100};
  const Order sell{2, Side::Sell, 10075, 50};

  printOrder(buy);
  printOrder(sell);
  std::cout << "buy.isBuy() = " << buy.isBuy() << '\n';
  std::cout << "sell.isBuy() = " << sell.isBuy() << '\n';
  return 0;
}
