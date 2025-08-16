# Arbitrage

This is a simple arbitrage detector. An arbitrage is a situtaion in which you can gain profit without any net exchange of stocks or goods. There are 2 versions in this implemenation, First one is as follows

Consider the following example. Individual A is willing to buy the package deal of Stock of company X "minus" the Stock of company Y for 10 dollars. Individual B is willing to pay 20 dollars to buy Company Y's stock and sell Company Z's stock as a package deal. Individual C is willing to buy company Z's stock and sell company X's stock for 5 dollars. "This is the best trade deal in the history of trade deals, maybe ever." Why? If you take all three deals, you have neither bought nor sold any stock but made off with 5 dollars. This is arbitrage.

Now, suppose our market also has such package deals expressed as linear combinations of stocks being bought and sold. Your algorithm should be able to detect all such opportunities where the trade deals executed collectively result in no stock being bought or sold, and yet, a net profit (non-zero). In the case of many such possibilities, you should choose the one which maximises profit. Each order should be output similar to part 1 (but with stock_name replaced with the entire stock structure linear combination).

Input data stream will consist of lines in the following format.
```bash
 <stock_name_1> <quantity_1> <stock_name_2>....<quantity_n> <price> <s/b>#
```
Now there may be multiple arbitrages possible, Suppose in you active counter of orders, you have order number 1,2,3,4,5 then there may be an arbitrage between 1,2,5 and 1,4,5. So you have to identify the max profit arbitrage among all of them and print that one in reverse order .i.e if 1,2,5 form max arbitrage then print them in the order 5,2,1. Also once an arbitrage is accepted the orders in that arbitrage have to removed from your active orders as they cannot be used again in future orders.

Let's see a simple example. 

```bash
  X 1 Y -1 10 b#
  Y 1 Z -1 10 b#
  Z 1 X -1 10 b#
```
The output will be 

```bash
  No Trade
  No Trade
  Z 1 X -1 10 s
  Y 1 Z -1 10 s
  X 1 Y -1 10 s
```

So for a given order is you haven't found any arbitrage or if the order is cancelled(will be discussed in detail later) then "No Trade" has to printed


In the advances version, you will have quantities associated with the orders

```bash
<stock_name_1> <quantity_1> <stock_name_2>....<quantity_n> <price> <quantity> <s/b>#
```

In this version, we will allow each order to have more than one quantity of each deal available. Additionally, we will also store the entire order book. That is, an order will come in that can have multiple stocks or stock structures to buy/sell at a particular price. For instance, ASML 1 100 10 b# is a deal to buy 10 stocks of ASML for $100/stock. Also, a buy order of ASML 1 120 10 b# will NOT cancel this order. An order of ASML 1 100 20 s# will cancel the ASML 1 100 10 b# order and be reduced to ASML 1 100 10 s#, and will NOT affect the ASML 1 120 10 b# order (another way in which our algorithm can make money).

It might look very similar to previous part but there is lot more complexity here. Here there is a provision to accept partial orders. A simple example is as follows

```bash
  A 1 10 5 b#
  B 1 10 3 b#
  C 1 10 4 b#
  A -1 B -1 C -1 2 b#
```

Output is 
```bash
  No trade
  No Trade
  No Trade
  A -1 B -1 C -1 2 s
  C 1 10 2 s
  B 1 10 2 s
  A 1 10 2 s
```

So clearly you can see that after this arbitrage still you have 3 A's 1 B 2 C's So thery can be used in future arbitrages.

This covers most of the problem statement, but to get a clear idea about the cancellation rules, you can refer to the README files of both Basic and Advanced Version, these give you a more clear idea of what how these work.

# Challenges
After you go through the problem statement thouroghly you will understand that it is not very trival, following are some challenges which you might face
* How to identify the arbitrage, more precisely, how will you check all possiblities and detect an arbitrge.
* How will you deal with both cancellation and arbitrage parallelly, Especially in Advanced version you might encounter cases where some part of the order is cancelled and some part is involved in an arbitrage.
* How will you improve the time complexity, rather how will you improve the average case?? You have to write your algorithm in such a way that it can atleast detect some waste iterations(those ones in which there is no scope of finding an arbitrage) and make the trading decision as soon as possible.

These are some very common challenges which you face while writing this code, 
there will be many more challenges while debugging and writing the algorithm.

# Run Instructions
 For the Basic Version
 * Copy your input into data2.txt given in inputs folder
 * Run the following command on terminal
 ```bash
 make runpart2
 ```
 * Your output will be displayed in output.txt
For the Advanced Version
* Copy your input into data3.txt given in inputs folder
* Run the following command
```bash
make runpart3
```
* Your output will be displayed in output.txt

# Port issue Handling
Remember for the basic version the time complexity if O(2^n) and for advanced version time complexity is O((q1+1)(q2+1)(q3+1)....(qn+1)). So basically it is of exponential order. So while you are testing, if the execution has not yet completed, but you want to run some other test case, then just change the port number in receiver.h and iml.cpp .
```bash
const int PORT = 8888;
```
You can choose other ports like 8888,8889,8881,etc.. 
Or you can kill the current process 
```bash
lsof -i :8888 // this will help to find the PID for process running in port 8888
```
```bash
kill -9 <PID> // Then kill the process
```
