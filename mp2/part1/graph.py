class TreeNode():
    def __init__(self, id,num, level, currl):
        self.children = {}
        self.id = id
        self.num=num
        self.level= level
        self.currl=currl

    def addchild(self,child, weight,letter):
        self.children[child]=(weight,letter)

    def __lt__(self, other):
        return self.num < other.num

