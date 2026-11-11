#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

enum class Side
{
  Buy,
  Sell
};

struct Order
{
  std::uint64_t id;
  Side side;
  std::int64_t price;
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
  std::cout << "id=" << o.id << " " << toString(o.side) << " price=" << o.price << " qty=" << o.qty << '\n';
}

int main()
{
  std::vector<Order> orders = {
      {1, Side::Buy, 10050, 100},
      {2, Side::Sell, 10100, 50},
      {3, Side::Buy, 10075, 200},
      {4, Side::Sell, 10060, 75},

  };
  std::cout << "Before sorting: \n";
  for (const Order &o : orders)
  {
    printOrder(o);
  }

  std::sort(orders.begin(), orders.end(), [](const Order &a, const Order &b)
            { return a.price < b.price; });

  std::cout << "after sorting by price: \n";
  for (const Order &o : orders)
  {
    printOrder(o);
  }
  return 0;
}