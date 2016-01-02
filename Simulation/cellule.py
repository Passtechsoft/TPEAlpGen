from bge import *
from bgl import *
from mathutils import *
from mathutils.kdtree import *

class Cellule():
	radius = 1.0
	def __init__(self, position):
		self.position = position
		self.velocityList = []
		self.velocity = Vector((0, 0))
		self.adjacentCells = []
		self.culled = False
		self.color = 0

	def appendFrontCells(self, frontCells):
		for cell in self.adjacentCells:
			if not cell.culled:
				frontCells.append(cell)

	def getAdjacentCells(self, tree, cellsList):
		self.adjacentCells.clear()
		cellsIndexes = tree.find_range(self.position.to_3d(), 1.5)
		self.count = len(cellsIndexes)
		for p in cellsIndexes:
			i = p[1]
			cell = cellsList[i]
			if cell is not self:
				self.adjacentCells.append(cell)

	def draw(self):
		glColor3f(1, 0, 1)
		glBegin(GL_LINES)
		for cell in self.adjacentCells:
			glVertex3f(cell.position.x, cell.position.y, 0)
			glVertex3f(self.position.x, self.position.y, 0)
		glEnd()

		glColor3f(self.color / 10, 0, 0)
		glPointSize(5)
		glBegin(GL_POINTS)
		glVertex3f(self.position.x, self.position.y, 0)
		glEnd()

	def setVelocity(self, velocity):
		self.velocityList.append(velocity)

	def applyVelocity(self):
		# on applique la velocité dans les cellules adjacentes
		if len(self.velocityList) == 0:
			return

		vel = Vector((0, 0))
		for v in self.velocityList:
			vel += v
		vel /= len(self.velocityList)

		for cell in self.adjacentCells:
			if cell.culled:
				continue

			dist = (cell.position - self.position).length
			factor = 1
			if dist > 0:
				factor = 1 / dist ** 2
			cell.velocityList.append(vel * factor)

			self.culled = True

	def updateTransform(self):
		if len(self.velocityList) == 0:
			return

		finalVelocity = Vector((0, 0))
		for vel in self.velocityList:
			finalVelocity += vel
		finalVelocity /= len(self.velocityList)
		self.position += finalVelocity
		self.velocityList.clear()
