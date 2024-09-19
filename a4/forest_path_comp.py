class Node:
    def __init__(self, value):
        self.value = value
        self.parent = self
        self.rank = 0  # Rank is used to keep the tree shallow

class DisjointSet:
    def __init__(self):
        self.map = {}

    def make_set(self, x):
        if x not in self.map:
            self.map[x] = Node(x)

    def find_set(self, x):
        if x in self.map:
            node = self.map[x]
            if node.parent != node:
                node.parent = self._find_root(node.parent)  # Path Compression
            return node.parent.value
        return None

    def _find_root(self, node):
        if node.parent != node:
            node.parent = self._find_root(node.parent)
        return node.parent

    def union(self, x, y):
        if x in self.map and y in self.map:
            root_x = self._find_root(self.map[x])
            root_y = self._find_root(self.map[y])

            if root_x != root_y:
                if root_x.rank > root_y.rank:
                    root_y.parent = root_x
                elif root_x.rank < root_y.rank:
                    root_x.parent = root_y
                else:
                    root_y.parent = root_x
                    root_x.rank += 1

            print(f"After union({x}, {y}):")
            self.display()

    def display(self):
        representatives = {}
        for key, node in self.map.items():
            rep = self.find_set(key)
            if rep not in representatives:
                representatives[rep] = []
            representatives[rep].append(key)
        
        for rep, nodes in representatives.items():
            print(f"  Set representative {rep}: {nodes}")

# Simulate the operations
ds = DisjointSet()

# Step 1: make-set(i) for i in 1,2,...,16
for i in range(1, 17):
    ds.make_set(i)

# Step 2: union(i, i+1) for i in 1,3,...,15
for i in range(1, 16, 2):
    ds.union(i, i + 1)

# Step 3: union(i, i+2) for i in 1, 5, 9, 13
for i in [1, 5, 9, 13]:
    ds.union(i, i + 2)

# Step 4: union(1,5); union(11,13); union(1,10)
ds.union(1, 5)
ds.union(11, 13)
ds.union(1, 10)

# Step 5: find-set(2) and find-set(16)
find_set_2 = ds.find_set(2)
find_set_16 = ds.find_set(16)

# Display the final results of find-set
result_2 = f"find-set(2): {find_set_2}"
result_16 = f"find-set(16): {find_set_16}"

