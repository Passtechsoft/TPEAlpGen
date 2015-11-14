from bge import *
from mathutils import *
from mathutils.kdtree import *

class Cellule(types.KX_GameObject):
	radius = 1.0
	def __init__(self, base, position, globalVel):
		self.globalVel = globalVel
		self.cposition = position
		self.velocity = globalVel
		self.previousVel = Vector((0, 0))

	def handleCollision(self, tree, celluleList):
		celluleIndexes = tree.find_range(self.cposition.to_3d(), Vector((1.5, 1.5)).length)
		count = len(celluleIndexes)
		deltaVel = Vector((0, 0))
		for p in celluleIndexes:
			i = p[1]
			cel = celluleList[i]
			deltaVel += cel.previousVel / 2
		deltaVel /= count
		self.velocity += deltaVel

	def updateTransform(self):
		self.previousVel = self.velocity
		self.cposition += self.velocity
		self.velocity = Vector((0, 0))# self.globalVel
		self.worldPosition.xy = self.cposition
