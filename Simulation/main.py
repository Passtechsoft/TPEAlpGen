from bge import *
from mathutils import *
from mathutils.kdtree import *
from math import *

from cellule import Cellule

size = 10
gap = 1
objectname = "Sphere"
globalVect = Vector((0, 0.2))

tree = None
cellsList = [None] * size ** 2

sce = logic.getCurrentScene()

def initialize(cont):
	own = cont.owner
	for xi in range(size):
		for yi in range(size):
			x = xi * gap
			y = yi * gap

			# Set sphere position.
			position = Vector((x, y))
			# Subclass Cellule proxy on.
			cell = Cellule(position)
			# Add the cellule in list.
			cellsList[xi + size * yi] = cell
	#for i in range(size):
		#factor = sin((i / size) * pi * 2) + 0.5
		#cellsList[i].previousVel = globalVect * factor
	for i in range(size):
		#factor = sin((i / size) * pi * 2) + 0.5
		if i > 10 and i < 20:
			factor = 1
		else:
			factor = 0
		cellsList[size ** 2 - i - 1].previousVel = -globalVect * factor
	sce.post_draw = [render]

def rebuildCelluleMap():
	global tree
	tree = KDTree(size ** 2)
	for i, cell in enumerate(cellsList):
		tree.insert(cell.position.to_3d(), i)
	tree.balance()

def frame():
	rebuildCelluleMap()

	for cell in cellsList:
		cell.culled = False
		cell.getAdjacentCells(tree, cellsList)

	origcell = cellsList[0]
	origcell.setVelocity(Vector((.1, .1)) * 1)
	collisionProcess(origcell)

	for cell in cellsList:
		cell.updateTransform()

def render():
	for cell in cellsList:
		cell.draw()

def collisionProcess(origcell):
	frontCells = [origcell]

	i = 0
	while len(frontCells):
		for cell in frontCells:
			cell.color = i
			cell.applyVelocity()

		# Creation d'un nouveau front d'onde
		# Liste temporaire des cellules de frontière de l'onde.
		tempFrontCells = []
		for cell in frontCells:
			# Pour chaque cellules on ajoute leurs cellules adjacentes non calculées.
			cell.appendFrontCells(tempFrontCells)
		# La nouvelle liste devient l'ancienne.
		frontCells = tempFrontCells

		i += 1
	print("done with", i, "front iteration")
