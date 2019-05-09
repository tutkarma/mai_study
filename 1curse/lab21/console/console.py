def read_graph():
    vertex_count = int(input('Кол-во вершин:'))
    edge_count = int(input('Кол-во ребер:'))
    graph = {}
    for edge in range(edge_count):
        vrtx1, vrtx2, weight = [int(x) for x in input().split()]
        if vrtx1 not in graph:
            graph[vrtx1] = {vrtx2:weight}
        else:
            graph[vrtx1][vrtx2] = weight

    for vrtx1 in list(graph):
        for vrtx2 in list(graph[vrtx1]):
            if vrtx2 not in graph:
                graph[vrtx2] = {-1:0}

    return graph


def top_sort(graph):  # алгоритм Кана

    graph_buf = graph
    # для каждой вершины находим число входящих в нее ребер
    in_degree = {}
    for vrtx in graph_buf:
        in_degree.setdefault(vrtx, 0)
        for neighbor in graph_buf[vrtx]:
            if neighbor != -1:
                in_degree[neighbor] = in_degree.get(neighbor, 0) + 1

    # находим вершины без входящих ребер
    empty = {vrtx for vrtx, count in in_degree.items() if count == 0}

    result = []
    while empty:
        # берем случайную вершину из empty
        vrtx = empty.pop()
        result.append(vrtx)

        # удаляем ребра, исходящие из нее
        for neighbor in graph_buf.get(vrtx, []):
            if neighbor != -1:
                in_degree[neighbor] -= 1

                # если смежная вершина имеет больше входящих ребер, добавляем ее в empty
                if in_degree[neighbor] == 0:
                    empty.add(neighbor)

    if len(result) != len(in_degree):
        return None # не является направленным ациклическим графом
    else:
        return result


def max_path_in_graph(graph, startnode, endnode):
    order = top_sort(graph)

    LOWDIST=-99999999999999999
    dist = dict((x, LOWDIST) for x in graph.keys())
    dist[startnode] = 0

    comesfrom = dict()
    for node in order:
        for nbr in graph[node]:
            if nbr != -1:
                if dist[nbr] <= dist[node] + graph[node][nbr]:
                    dist[nbr] = dist[node] + graph[node][nbr]
                    comesfrom[nbr] = node

    maxpath = [endnode]
    while maxpath[-1] != startnode:
        maxpath.append(comesfrom[maxpath[-1]])
    maxpath.reverse()

    return dist[endnode], maxpath


if __name__ == "__main__":
    graph = read_graph()
    #print(graph)
    #sorted_graph = top_sort(graph)
    #print(sorted_graph)
    startnode = int(input('Начальная вершина:'))
    endnode = int(input('Конечная вершина:'))
    maxLength, maxPath = max_path_in_graph(graph, startnode, endnode)
    print(maxLength)
    print(maxPath)
