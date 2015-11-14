from bge import *
from mathutils import *
from mathutils.kdtree import *

from cellule import Cellule

size = 2
gap = 1
objectname = "Sphere"
globalVect = Vector((.1, 0))

tree = None
celluleList = [None] * size ** 2

sce = logic.getCurrentScene()

def initialize(cont):
	own = cont.owner
	for xi in range(size):
		for yi in range(size):
			x = xi * gap
			y = yi * gap

			# Create new BGE object.
			sphere = sce.addObject(objectname, own)
			# Set sphere position.
			position = Vector((x, y))
			sphere.worldPosition.xy = position
			# Subclass Cellule proxy on.
			cel = Cellule(sphere, position, globalVect)
			# Add the cellule in list.
			celluleList[xi + size * yi] = cel

def rebuildCelluleMap():
	global tree
	tree = KDTree(size ** 2)
	for i, cel in enumerate(celluleList):
		tree.insert(cel.position, i)
	tree.balance()

def frame():
	rebuildCelluleMap()

	for cel in celluleList:
		cel.handleCollision(tree, celluleList)

	for cel in celluleList:
		cel.updateTransform()
