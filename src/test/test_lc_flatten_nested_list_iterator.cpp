#include "test_optional.h"

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <vector>

using namespace std;

///**
// * // This is the interface that allows for creating nested lists.
// * // You should not implement it, or speculate about its implementation
// * class NestedInteger {
// *   public:
// *     // Return true if this NestedInteger holds a single integer, rather than a nested list.
// *     bool isInteger() const;
// *
// *     // Return the single integer that this NestedInteger holds, if it holds a single integer
// *     // The result is undefined if this NestedInteger holds a nested list
// *     int getInteger() const;
// *
// *     // Return the nested list that this NestedInteger holds, if it holds a nested list
// *     // The result is undefined if this NestedInteger holds a single integer
// *     const vector<NestedInteger> &getList() const;
// * };
// */
//
//class NestedIterator {
//public:
//    NestedIterator(vector<NestedInteger>& nestedList) {
//
//    }
//
//    int next() {
//
//    }
//
//    bool hasNext() {
//
//    }
//};
//
///**
// * Your NestedIterator object will be instantiated and called as such:
// * NestedIterator i(nestedList);
// * while (i.hasNext()) cout << i.next();
// */

BOOST_AUTO_TEST_SUITE(s_test_lc_flatten_nested_list_iterator)

BOOST_AUTO_TEST_CASE(c_test_lc_flatten_nested_list_iterator)
{
    //NestedIterator i(nestedList);
    //while (i.hasNext()) cout << i.next();

}

BOOST_AUTO_TEST_SUITE_END()





//解答


//
//方法一：深度优先搜索
//思路
//
//嵌套的整型列表是一个树形结构，树上的叶子节点对应一个整数，非叶节点对应一个列表。
//
//在这棵树上深度优先搜索的顺序就是迭代器遍历的顺序。
//
//我们可以先遍历整个嵌套列表，将所有整数存入一个数组，然后遍历该数组从而实现 \texttt{ next }next 和 \texttt{ hasNext }hasNext 方法。
//
//代码
//
//class NestedIterator {
//private:
//    vector<int> vals;
//    vector<int>::iterator cur;
//
//    void dfs(const vector<NestedInteger>& nestedList) {
//        for (auto& nest : nestedList) {
//            if (nest.isInteger()) {
//                vals.push_back(nest.getInteger());
//            }
//            else {
//                dfs(nest.getList());
//            }
//        }
//    }
//
//public:
//    NestedIterator(vector<NestedInteger>& nestedList) {
//        dfs(nestedList);
//        cur = vals.begin();
//    }
//
//    int next() {
//        return *cur++;
//    }
//
//    bool hasNext() {
//        return cur != vals.end();
//    }
//};
//复杂度分析
//
//时间复杂度：初始化为 O(n)O(n)，\texttt{ next }next 和 \texttt{ hasNext }hasNext 为 O(1)O(1)。其中 nn 是嵌套的整型列表中的元素个数。
//
//空间复杂度：O(n)O(n)。需要一个数组存储嵌套的整型列表中的所有元素。
//
//方法二：栈
//思路
//
//我们可以用一个栈来代替方法一中的递归过程。
//
//具体来说，用一个栈来维护深度优先搜索时，从根节点到当前节点路径上的所有节点。由于非叶节点对应的是一个列表，我们在栈中存储的是指向列表当前遍历的元素的指针（下标）。每次向下搜索时，取出列表的当前指针指向的元素并将其入栈，同时将该指针向后移动一位。如此反复直到找到一个整数。循环时若栈顶指针指向了列表末尾，则将其从栈顶弹出。
//
//下面的代码中，\texttt{ C++ }C++ 和 \texttt{ Java }Java 的栈存储的是迭代器，迭代器可以起到指向元素的指针的效果，\texttt{ Go }Go 的栈存储的是切片（视作队列），通过将元素弹出队首的操作代替移动指针的操作。
//
//代码
//
//
//class NestedIterator {
//private:
//    // pair 中存储的是列表的当前遍历位置，以及一个尾后迭代器用于判断是否遍历到了列表末尾
//    stack<pair<vector<NestedInteger>::iterator, vector<NestedInteger>::iterator>> stk;
//
//public:
//    NestedIterator(vector<NestedInteger>& nestedList) {
//        stk.emplace(nestedList.begin(), nestedList.end());
//    }
//
//    int next() {
//        // 由于保证调用 next 之前会调用 hasNext，直接返回栈顶列表的当前元素，然后迭代器指向下一个元素
//        return stk.top().first++->getInteger();
//    }
//
//    bool hasNext() {
//        while (!stk.empty()) {
//            auto& p = stk.top();
//            if (p.first == p.second) { // 遍历到当前列表末尾，出栈
//                stk.pop();
//                continue;
//            }
//            if (p.first->isInteger()) {
//                return true;
//            }
//            // 若当前元素为列表，则将其入栈，且迭代器指向下一个元素
//            auto& lst = p.first++->getList();
//            stk.emplace(lst.begin(), lst.end());
//        }
//        return false;
//    }
//};
//复杂度分析
//
//时间复杂度：初始化和 \texttt{ next }next 为 O(1)O(1)，\texttt{ hasNext }hasNext 为均摊 O(1)O(1)。
//
//空间复杂度：O(n)O(n)。最坏情况下嵌套的整型列表是一条链，我们需要一个 O(n)O(n) 大小的栈来存储链上的所有元素。
//










//
//各位题友大家好！ 今天是 @负雪明烛 坚持日更的第 58 天。今天力扣上的每日一题是「341.扁平化嵌套列表迭代器」。
//
//题意解析
//今天的题意略难理解，需要我翻译一下，理解题意的朋友请跳过。
//
//本题定义了一个类 NestedInteger ，这个类可以存储 int  或 List<NestedInteger> ；所以称它是一个「嵌套列表」。类似于一棵多叉树，每个节点都可以有很多子节点。
//
//它有三个方法：
//
//isInteger() ，判断当前存储的对象是否为 int；
//getInteger(), 如果当前存储的元素是 int 型的，那么返回当前的结果 int，否则调用会失败；
//getList() ，如果当前存储的元素是 List<NestedInteger> 型的，那么返回该 List，否则调用会失败。
//而「扁平化嵌套列表迭代器」说的是，我们需要设计一个迭代器，这个迭代器是把「嵌套列表」铺平（拆包）成各个 int，然后每次调用 hasNext() 来判断是否有下一个整数，通过 next() 返回下一个整数。
//
//注意迭代器是一种按照特定顺序对数据结构遍历的方式，它的调用方式是：
//
//
//i, v = NestedIterator(nestedList), []
//while i.hasNext() :
//    v.append(i.next())
//    解题思路
//    本文有两种主要的思路：
//
//    在构造函数中提前「扁平化」整个嵌套列表；
//    在调用 hasNext() 或者 next() 方法的时候扁平化当前的嵌套的子列表。
//    一、递归：构造函数中提前「扁平化」整个嵌套列表
//    这是最简单的方法，但是我认为这不是面试官想要的方法。
//
//    在构造函数中提前扁平化整个嵌套列表。那么在 hasNext() 或者 next() 可以很简单地返回迭代器位置的 int。
//
//    因为这个嵌套的数据结构有点类似于多叉树，所以我们可以按照类似地遍历思路：递归。
//
//    承载遍历结果的数据结构可以使用数组，那么另外需要一个整数标记当前的迭代器指向的位置；也可以使用一个队列，每次调用 next() 方法的时候从队列的开头弹出一个元素。
//
//    递归的思路比较简单，我用的是队列：
//
//    遍历输入的「嵌套列表」所有的元素，判断每个元素是 int 还是 list；
//    如果当前元素是 int，放入队列尾部；
//    如果当前元素是 list，那么需要对当前 子list 继续递归；
//    其余代码比较简单，不作赘述。
//
//    Python
//
//    class NestedIterator(object) :
//    def dfs(self, nests) :
//    for nest in nests :
//if nest.isInteger() :
//    self.queue.append(nest.getInteger())
//else :
//    self.dfs(nest.getList())
//
//    def __init__(self, nestedList) :
//    self.queue = collections.deque()
//    self.dfs(nestedList)
//
//    def next(self) :
//    return self.queue.popleft()
//
//    def hasNext(self) :
//    return len(self.queue)
//    时间复杂度：构造函数的时间复杂度是 O(N)O(N)； next() 和 hasNext() 的时间复杂度是 O(1)O(1)。
//    空间复杂度：O(N)O(N)。
//    二、迭代：调用 hasNext() 或者 next() 方法的时候扁平化当前的嵌套的子列表
//    这个方法更加有挑战性，也是这个题最正确的解法。因为对于大部分情况，我们希望迭代器能够一边迭代一边获取当前的结果，而不是提前初始化好。
//
//    把递归方法 转 迭代方法，我们需要用到「栈」。
//
//    在递归方法中，我们在遍历时如果遇到一个嵌套的 子list，就立即处理该 子list，直到全部展开；
//    在迭代方法中，我们不需要全部展开，只需要把 当前list 的所有元素放入 list 中。
//
//    由于「栈」的先进后出的特性，我们需要逆序在栈里放入各个元素。
//
//    处理流程分为两步：
//
//    在构造函数中应该初始化，把当前列表的各个元素（不用摊平）逆序放入栈中。
//    在 hasNext() 方法中，访问（不弹出）栈顶元素，判断是否为 int：
//    如果是 int 那么说明有下一个元素，返回 true；然后 next() 就会被调用，把栈顶的 int 弹出；
//    如果是 list 需要把当前列表的各个元素（不用摊平）逆序放入栈中。
//    如果栈为空，那么说明原始的嵌套列表已经访问结束了，返回 false。
//    算法整体的流程，通过举例说明。假如输入[1, [2, 3]] 。
//
//    1. 在构造函数中：栈里面放的应该是 stack = [[2, 3], 1]
//    2. 在调用 hasNext() 方法时，访问栈顶元素是 1，为 int，那么直接返回 true;
//3. 然后调用 next() 方法，弹出栈顶元素 1；
//4. 再调用 hasNext() 方法时，访问栈顶元素是[2, 3]，为 list，那么需要摊平，继续放到栈中。
//当前的栈是 stack = [3, 2]
//5. 然后调用 next() 方法，弹出栈顶元素 2；
//6. 然后调用 next() 方法，弹出栈顶元素 3；
//7. 再调用 hasNext() 方法时，栈为空，因此返回 false，迭代器运行结束。
//这里需要说一下为什么在 hasNext() 方法中摊平 list，而不是在 next() 方法中。比如对于 [[]] 的输入， hasNext()  方法是判断其中是否有 int 元素了，则必须把内层的 list 摊平来看，发现是空的，返回 false。
//
//代码如下：
//
//PythonC++
//
//class NestedIterator(object) :
//
//    def __init__(self, nestedList) :
//    self.stack = []
//    for i in range(len(nestedList) - 1, -1, -1) :
//        self.stack.append(nestedList[i])
//
//
//        def next(self) :
//        cur = self.stack.pop()
//        return cur.getInteger()
//
//        def hasNext(self) :
//        while self.stack :
//            cur = self.stack[-1]
//            if cur.isInteger() :
//                return True
//                self.stack.pop()
//                for i in range(len(cur.getList()) - 1, -1, -1) :
//                    self.stack.append(cur.getList()[i])
//                    return False
//                    时间复杂度：构造函数的最坏时间复杂度是 O(N)O(N)； next() 和 hasNext() 的最坏时间复杂度是 O(N)O(N)。
//                    空间复杂度：O(N)O(N)。
//                    刷题心得
//                    面试的时候需要写出方法二，今天的题目很好，请务必掌握呀。
//













