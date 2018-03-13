from graph import *
import queue
import copy
fact_1 = "AEDCA"
fact_2 = "BEACD"
fact_3 = "DADBD"
fact_4 = "BECBD"
fact_5 = "BABCE"
facts = [fact_1, fact_2, fact_3, fact_4, fact_5]
two_seq = []
comb_done = []
two_seq_map = {}
graph = {}
edges={}
edges[('A','B')]=1064
edges[('A','C')]=673
edges[('A','D')]=1401
edges[('A','E')]=277
edges[('B','C')]=958
edges[('B','D')]=1934
edges[('B','E')]=337
edges[('C','D')]=1001
edges[('C','E')]=399
edges[('D','E')]=387
edges[('B','A')]=1064
edges[('C','A')]=673
edges[('D','A')]=1401
edges[('E','A')]=277
edges[('C','B')]=958
edges[('D','B')]=1934
edges[('E','B')]=337
edges[('D','C')]=1001
edges[('E','C')]=399
edges[('E','D')]=387

levels=12
lets=['A','B','C','D','E']

start=['', '', '', '', '']

root= TreeNode(start,0,0,'',0)
num=0
graph[tuple(start)]= root
creatq=queue.Queue()
creatq.put(root)
ex=0
lvl=0
curr_beg = creatq.get().id
for l in lets:
    curr=copy.deepcopy(curr_beg)
    ex=0
    for i in range(5):
        if curr[i]+l == facts[i][0:len(curr[i])+1]:
            curr[i]=curr[i]+l
            ex=1
    if not ex:
        continue
    num=num+1
    x = TreeNode(curr, num,1,l,0)
    creatq.put(x)
    root.addchild(tuple(curr),0,l)
    graph[tuple(curr)]=x

lvl=1
while not creatq.empty():
    curr_beg = creatq.get()
    for l in lets:
        curr = copy.deepcopy(curr_beg.id)
        lvl=curr_beg.level
        ex = 0
        curwt = curr_beg.dist

        for i in range(5):
            if curr[i] + l == facts[i][0:len(curr[i]) + 1]:
                curr[i] = curr[i] + l
                ex = 1
        if not ex:
            continue
        tup=tuple(curr)
        if tup in graph:
            curr_beg.addchild(tup, 0,l)
            continue
        num=num+1
        x = TreeNode(curr, num,lvl+1,l,curwt+edges[(curr_beg.currl,l)])
        creatq.put(x)
        #print(curr,num)
        curr_beg.addchild(tup,edges[(curr_beg.currl,l)],l)
        graph[tup] = x
#print(graph[tuple(facts)].level)

#PUT LETTER IN TUPLE FOR CHILD NODE

sq=''

#BFS: ABEDCADBCDE
# frontier = queue.Queue()
# lq= queue.Queue()
# frontier.put(graph[tuple(start)])
# lq.put('')
# while not frontier.empty():
#     curr = frontier.get()
#     sq=lq.get()
#     if curr.id == facts:
#         print(curr.dist,sq)
#     for neighb, wt in curr.children.items():
#         for n, m in graph.items():
#             if n == neighb:
#                 frontier.put(m)
#                 lq.put(sq+wt[1])
#                 break

#NAIVE ASTAR FOR SEQUENCE
frontier = queue.PriorityQueue()
frontier.put((0,graph[tuple(start)],'',0))
seen=0
expand=0
while not frontier.empty():
    curr = frontier.get()
    expand=expand+1
    seen=seen+1
    sq=curr[2]
    curwt=curr[3]
    if curr[1].id == facts:
        print("Lowest num of factories:")
        print(sq, len(sq), curwt)
        print("Nodes expanded:")
        print(expand)
        break
    for neighb, wt in curr[1].children.items():
        for n, m in graph.items():
            if n == neighb:
                frontier.put((m.level+seen,m,sq+wt[1],curwt+wt[0]))
                break

#ASTAR FOR SHORTEST MILES
frontier = queue.PriorityQueue()
frontier.put((0,graph[tuple(start)],'',0))
seen=0
expand=0
#test=[]
while not frontier.empty():
    curr = frontier.get()
    expand=expand+1

    seen=seen+1
    sq=curr[2]
    curwt=curr[3]
    if curr[1].id == facts:
        print("Lowest num of miles")
        print(sq, len(sq), curwt)
#        test.append(curwt)
        print("Nodes expanded:")
        print(expand)
        break
    for neighb, wt in curr[1].children.items():
        for n, m in graph.items():
            if n == neighb:
                frontier.put((curwt+wt[0],m,sq+wt[1], curwt+wt[0]))
                break

#print(min(test))
def heuristic1():
    return 1