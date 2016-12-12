# This file contains the definition and Implementation of CornerNode and the routing algorithm.

class CornerNode:

	def __init__(self, id, x, y):
		self.id = id
		self.x = x
		self.y = y
		self.neighbors = []
		self.path = []
		self.path.append(self)
		self.distance = 0
	# update the current shortest distance from the start vertex to the current vertex
	def updateDistance(self, distance):
		self.distance = distance
	# update the shortest path from the start vertex to the current vertex
	def updatePath(self, a):
		self.path = []
		for item in a.path:
			self.path.append(item)
		self.path.append(self)
	# clear the path of current vertex
	def clearPath(self):
		self.path[:] = []
		self.path.append(self)
		self.distance = 2147483647
	# print the shortest path from the start vertex to current vertex by id
	def printPath(self):
		for item in self.path:
			print item.id
	# add adjacent vertex to the node
	def addNeighbor(self, neighbor):
		self.neighbors.append(neighbor)

import math
from sets import Set
import sqlite3
cornerSet = set([])
currentstate = 0

# Create map from the database by vertices and edges
def createMap(database, destinationID):
	dict = {}
	cx = sqlite3.connect(database)
	for row in cx.execute("select * from vertices"):
		cornerNode = CornerNode(row[0], row[1], row[2])
		cornerSet.add(cornerNode)
		dict[row[0]] = cornerNode
	for row in cx.execute("select * from edges"):
		dict[row[0]].addNeighbor(dict[row[1]])
	carinfo = cx.execute("select * from car_locations where Car_ID = 1001").fetchone()
	start = dict[carinfo[1]]
	destination = dict[destinationID]
	currentstate = carinfo[2]
	instruction = route(start, destination)
	print instruction
	cx.execute("UPDATE car_locations set Turn_by_Turn = ?, Dir = ? where Car_ID = ?", (instruction, currentstate, 1001))
	cx.commit()


# The main function for get the shortest path from the start vertex to the destination
def route(start, destination):
	for item in cornerSet:
		item.clearPath()
	start.updateDistance(0)
	traversal(start, cornerSet)
	destination.printPath()
	print destination.distance
	return encodePath(destination)

# Routing Algorithm using BFS(Dijkstra) 
def traversal(start, nodes):
	nodes.remove(start)
	nextstart = None
	for item in start.neighbors:
		if item in nodes:
			dis = math.sqrt((item.x - start.x)*(item.x - start.x) + (item.y - start.y)*(item.y - start.y)) + start.distance
			if dis < item.distance:
				item.updateDistance(dis)
				item.updatePath(start)
	if len(nodes) != 0:
		min_d = 2147483647
		for item in nodes:
			if item.distance <= min_d:
				nextStart = item
				min_d = item.distance
		traversal(nextStart, nodes)

# Encoding the path and instructions to string
# 4 digits of beacon id, 1 digit of instruction (0-forward; 1-turn right; 2-back; 3-turn left; 4-stop)
def encodePath(destination):
	instructions = str(destination.path[0].id);
	for i in range(1, len(destination.path)):
		instr = getInstruction(getDirection(destination.path[i - 1], destination.path[i]))
		instructions += str(instr)
		instructions += str(destination.path[i].id)
		if instr != 2:
			currentstate = getDirection(destination.path[i - 1], destination.path[i])
		print currentstate
	instructions += "4";
	return instructions

# Get the direction at the corner
# 1-(0,1); 2-(1, 0); 3-(0, -1); 4-(-1, 0)
def getDirection(node1, node2):
	if node1.x == node2.x:
		if node2.y > node1.y: return 1
		else: return 3
	if node1.y == node2.y:
		if node2.x > node1.x: return 2
		else: return 4
		
# Get the instruction at the corner
# 0-forward; 1-turn right; 2-back; 3-turn left; 4-stop
def getInstruction(state2):
	return (state2 - currentstate + 4) % 4
	

createMap("locationsNroutes.db", 2007)


