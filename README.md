## 大二上-数据结构I-课程设计
### 1.设计思路
**程序界面是一个二维平面图，蛇的身体可以看作是链表的节点，但蛇吃到食物时，就增加一节链表节点，食物也相应地在边界内随机生成，取得上下左右键的ASCII码的键值，通过switch语句响应相应的键值对，实现蛇的方向获取，通过局部打印空格与蛇身节点的移动，来实现局部更新可避免全局绘制造成的闪烁问题，也能提高性能，获取游戏一个实体的坐标需要得到当前窗口的句柄，并重写gotoxy()函数，在绘制时也要注意绘制重合的问题，把相应的功能模块化，结构化，封装成一个个函数，像蛇逻辑有自噬死亡，撞墙死亡，吃食，变长等，在相应的自定义逻辑函数中调用各自的函数，主函数则主要写下简短的循环框架，直到游戏结束
