class Node:
    def __init__(self, value):
        self.value = value
        self.next = None
        self.representative = self  # Initially, each node is its own representative


class DisjointSet:
    def __init__(self):
        self.map = {}

    def make_set(self, x):
        if x not in self.map:
            self.map[x] = Node(x)

    def find_set(self, x):
        if x in self.map:
            return self.map[x].representative.value
        return None

    def union(self, x, y):
        if x in self.map and y in self.map:
            rep_x = self.map[x].representative
            rep_y = self.map[y].representative
            
            # Merge shorter list into the longer one
            if self.list_size(rep_x) >= self.list_size(rep_y):
                self.append_list(rep_x, rep_y)
            else:
                self.append_list(rep_y, rep_x)
            
            print(f"After union({x}, {y}):")
            self.display()

    def list_size(self, node):
        size = 0
        current = node
        while current:
            size += 1
            current = current.next
        return size

    def append_list(self, larger, smaller):
        current = smaller
        while current:
            current.representative = larger
            if current.next is None:
                current.next = larger.next
                larger.next = smaller
                break
            current = current.next

    def display(self):
        representatives = {}
        for key, node in self.map.items():
            rep = node.representative.value
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
print(f"find-set(2): {find_set_2}")
print(f"find-set(16): {find_set_16}")
ds.display()