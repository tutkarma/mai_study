# -*- coding: utf-8 -*-
from flask import render_template, flash, redirect
from flask import request
from app import app
import json


def top_sort(graph):
    graph_buf = graph
    in_degree = {}
    for vrtx in graph_buf:
        in_degree.setdefault(vrtx, 0)
        for neighbor in graph_buf[vrtx]:
            if neighbor != -1:
                in_degree[neighbor] = in_degree.get(neighbor, 0) + 1

    empty = {vrtx for vrtx, count in in_degree.items() if count == 0}

    result = []
    while empty:
        vrtx = empty.pop()
        result.append(vrtx)

        for neighbor in graph_buf.get(vrtx, []):
            if neighbor != -1:
                in_degree[neighbor] -= 1

                if in_degree[neighbor] == 0:
                    empty.add(neighbor)

    if len(result) != len(in_degree):
        return None
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

    return maxpath


@app.route('/')
@app.route('/index',methods=['GET', 'POST'])
def index():
    return render_template("index.html")

@app.route('/cytoscape-edge.js',methods=['GET', 'POST'])
def cytoscape():
    return render_template("cytoscape-edge.js")

@app.route('/code.js', methods=['GET', 'POST'])
def code():
    return render_template("code.js")

@app.route('/_find', methods=['GET', 'POST'])
def find():
    data = request.json
    startnode = int(data["startNode"])
    endnode = int(data["endNode"])
    graph_tmp = {}
    graph = {}
    try:
        graph_tmp = data["graph"]

        #string->int
        for node in graph_tmp.keys():
            for node2, weight in graph_tmp[node].items():
                if int(node) not in graph:
                    graph[int(node)] = {int(node2):int(weight)}
                else:
                    graph[int(node)][int(node2)] = int(weight)

        for vrtx1 in list(graph):
            for vrtx2 in list(graph[vrtx1]):
                if vrtx2 not in graph:
                    graph[vrtx2] = {-1:0}
    except:
        pass

    if request.method == "POST":
        try:
            return json.dumps(max_path_in_graph(graph, startnode, endnode))
        except:
            return
    else:
        return -1