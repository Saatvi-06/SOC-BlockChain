import networkx as nx
import matplotlib.pyplot as plt

def visualize(filename):
    G = nx.Graph()
    with open(filename, 'r') as f:
        for line in f:
            u, v = map(int, line.strip().split())
            G.add_edge(u, v)
    plt.figure(figsize=(12, 10))
    nx.draw(G, with_labels=True, node_size=300, node_color="skyblue", font_size=8, edge_color="gray")
    plt.title("P2P Network Visualization")
    plt.savefig("network.png")
    plt.show()

visualize("edges.txt")
