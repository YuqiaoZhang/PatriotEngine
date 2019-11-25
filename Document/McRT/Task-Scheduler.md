﻿## Task Scheduler

### Intel TBB

基于Tag: 2019_U9  

#### Fork-Join

#### Scheduling Algorithm

//在TBB中

一些结构体定义  
```
task //DAG简化得到的树中的节点
    parent //唯一后继 //除root_task以外，都不可能为NULL
    ref_count //前驱的个数
```

//将DAG（Directed Acyclic Graph，有向无环图）中每个节点（即Task）的后继（Successor）个数限制为1，并将该唯一的后继定义为父节点（Parent），从而将DAG简化为树  
//注：在该DAG简化得到的树中，前驱和后继的方向与常规的有向树定义中的方向恰好相反  

//只有没有前驱（Predecessor）的节点（即树中的叶节点）才可能在Deque上 //即被Spawn 

//Spawn Task可以看作是整个系统的原语（Primitive）

//Worker Thread的Spawn Task会添加到Worker Thread的Deque  

//在Task执行完毕（且未recyle）后，会以原子操作的方式递减parent(即唯一的后继）的ref_count  
//当parent的ref_count递减至0时（即parent不再有前驱），parent会被Spawn  

//根据TBB的设计，Task在被创建时，处于DAG中不可能被执行的部分，因此，在set_ref_count中以非原子操作的方式设置引用计数  
//可能同时 也具有 减少原子操作的次数，提升性能 的目的  
//
//在Debug版本中，TBB提供了es_ref_count_active标志位进行错误检查 //该标志位表示是否该Task的ref_count有可能正在被某个线程以原子操作的方式递减 //实际上即等价于是否有前驱  
//在Spawn Task前（prepare_for_spawning），Task的parent的es_ref_count_active会被设置  
//当parent的ref_count递减至0时，parent的es_ref_count_active会被清空  

//注：set_ref_count是多余的，可以基于allocate_child recycle_as_child_of ... 等确定  
//在prepare_for_spawning中，ref_count被同步到0


//state
//None -Allocate-> allocate -Spawn-> ready -BypassLoop-> executing -Recycle-> recycled -BypassLoop-> allocate  
//                                                          |->freed //TBB特有 //对内存分配进行了缓存  

//recycle_as_safe_continuation  
//set_ref_count比实际值大1  
//为避免 其它线程 在原Task的execute返回前 steal并execute了该Task的child 且将该Recycle的Task的ref_count递减至0 从而并发执行Recycle为continuation的Task的execute  
//当state从recycled转换为allocat时，该额外的引用计数会被递减 （注意：代码在case recycled:中并没有break，执行到case to_enqueue:下的tally_completion_of_predecessor时，递减引用计数）  
  

//何时停止
work steal的缺陷
//worker thread正在执行剩余的最后1个task
//master thread判断task并没有全部完成 转去执行其它线程的task（即master thread steal 其它线程的thread)
//worker thread执行完毕 //master thread理应可以返回 但是却在执行其它线程的task (显然，task的excute方法内部不可中断)

custom_scheduler.h  
local_wait_for_all  
```
local_wait_for_all
    
    process_bypass_loop
        t //Task Executing
        t_next //Task Bypass

        t_next = t->execute()

        switch(t->state)
            case executing:
            assert(t->ref_count == 0) //t会被释放 //显然不应当再有前驱
                tally_completion_of_predecessor
                    //由于原子操作较慢，进行了优化
                    bool completion_of_predecessor;
                    if(==1)
                    {
                        ref_count = 0
                        completion_of_predecessor = true;
                    }
                    else
                    {
                        if(Atomic_Add(ref_count,-1)>1)
                        {
                            completion_of_predecessor = false;
                        }
                        else
                        {
                            assert(ref_count==0);
                            completion_of_predecessor = true;
                        }
                    }

                    if(completion_of_predecessor)
                    {
                        spawn parent(//Successor)
                    }

                    //Bypass Parent 的优化


```

(2.\[Intel® Software 2019\]/Developer Reference/Task Scheduler/Scheduling Algorithm)


#### Continuation Passing


Continuation-passing Style / Blocking Style (2.\[Intel® Software 2019\]/Developer Reference/Task Scheduler)  

Continuation Passing (2.\[Intel® Software 2019\]/Developer Guide/The Task Scheduler/Useful Task Techniques/Continuation Passing)  

steal children / steal continuations (1.\[McCool 2012\]/8.5 Load Balancing)  

#### Scheduler Bypass
https://software.intel.com/en-us/node/506107  

#### task_list

### Patriot TBB


## Generic Parallel Algorithms

### Map

Latency

(1.\[McCool 2012\]/8.3 Recursive Implementation Of Map)

### Pipeline

Throughout

## Flow Graph

### Wavefront

(2.\[Intel® Software 2019\]/Design Patterns/Wavefront) 

## 参考文献

[1.\[McCool 2012\] Michael McCool, James Reinders, Arch Robison. "Structured Parallel Programming: Patterns for Efficient Computation." Morgan Kaufmann Publishers 2012.](http://parallelbook.com/)  

[2.\[Intel® Software 2019\] Intel® Software. "Intel® Threading Building Blocks Documentation." Intel Developer Zone 2019.](https://www.threadingbuildingblocks.org/docs/help/index.htm)  