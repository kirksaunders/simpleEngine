#include <cmath>

#include "astar/grid.hpp"

using namespace Math3D;

Grid::Grid() {
	grid = NULL;
	width = height = depth = 0;
}

Grid::Grid(int x, int y, int z) {
	width = x * 2;
	height = y * 2;
	depth = z * 2;
	grid = new Node*[(width + 1) * (height + 1) * (depth + 1)];
}

Grid::~Grid() {
	destroy();
}

void Grid::destroy() {
	if (grid != NULL) {
		int halfW, halfH, halfD;
		halfW = width / 2;
		halfH = height / 2;
		halfD = depth / 2;
		Node* node;
		for (int x = -halfW; x < halfW; x++) {
			for (int y = -halfH; y < halfH ; y++) {
				for (int z = -halfD; z < halfD; z++) {
					node = getNode(x, y, z);
					if (node != NULL) {
						delete node;
					}
				}
			}
		}
	}
	delete[] grid;
}

void Grid::setNode(int x, int y, int z, Node* node) {
	int halfW, halfH, halfD;
	halfW = width / 2;
	halfH = height / 2;
	halfD = depth / 2;
	x = x + halfW;
	y = y + halfH;
	z = z + halfD;
	grid[y*(width+1)*(depth+1) + x*(depth+1) + z] = node;
}

Node* Grid::getNode(int x, int y, int z) const {
	int halfW, halfH, halfD;
	halfW = width / 2;
	halfH = height / 2;
	halfD = depth / 2;
	if (x < -halfW || x > halfW || y < -halfH || y > halfH || z < -halfD || z > halfD) {
		return NULL;
	}
	x = x + halfW;
	y = y + halfH;
	z = z + halfD;
	return grid[y*(width+1)*(depth+1) + x*(depth+1) + z];
}

Node* Grid::getNode(const Vector4 pos) const {
	int x, y, z;
	x = floor(pos[0] + 0.5);
	y = floor(pos[1] + 0.5);
	z = floor(pos[2] + 0.5);
	int halfW, halfH, halfD;
	halfW = width / 2;
	halfH = height / 2;
	halfD = depth / 2;
	if (x < -halfW || x > halfW || y < -halfH || y > halfH || z < -halfD || z > halfD) {
		return NULL;
	}
	x = x + halfW;
	y = y + halfH;
	z = z + halfD;
	return grid[y*(width+1)*(depth+1) + x*(depth+1) + z];
}

void Grid::load(std::istream& ins) {
	if (grid != NULL) {
		destroy();
	}
	ins >> width >> height >> depth;
	int halfW, halfH, halfD;
	halfW = width;
	halfH = height;
	halfD = depth; 
	width *= 2;
	height *= 2;
	depth *= 2;

	grid = new Node*[(width + 1) * (height + 1) * (depth + 1)];
	bool b;
	for (int x = -halfW; x <= halfW; x++) {
		for (int y = -halfH; y <= halfH ; y++) {
			for (int z = -halfD; z <= halfD; z++) {
				ins >> b;
				if (b) {
					setNode(x, y, z, NULL);
				} else {
					setNode(x, y, z, new Node(Vector4(x, y, z)));
				}
			}
		}
	}
}

void Grid::save(std::ostream& outs) {
	int halfW, halfH, halfD;
	halfW = width / 2;
	halfH = height / 2;
	halfD = depth / 2; 
	outs << halfW << std::endl << halfH << std::endl << halfD << std::endl;
	if (grid == NULL) {
		return;
	}

	for (int x = -halfW; x <= halfW; x++) {
		for (int y = -halfH; y <= halfH ; y++) {
			for (int z = -halfD; z <= halfD; z++) {
				outs << (getNode(x, y, z) == NULL);
				if (halfH == halfH - 1 && halfD == halfD - 1) {
					outs << std::endl;
				} else {
					outs << " ";
				}
			}
		}
	}
}