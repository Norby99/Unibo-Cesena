package e2;

import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import sol2.Pair;

public class Logic {
	
	private final Map<Pair<Integer, Integer>, Integer> gridItems = new HashMap<>();
	private int gridDimension;
	private Random random = new Random();
	
	public Logic(final int size) {
		this.gridDimension = size;
		populateGrid();
	}
	
	private void populateGrid() {
		for (int i = 0; i < this.gridDimension; i++) {
			for (int j = 0; j < this.gridDimension; j++) {
				gridItems.put(new Pair<>(i,j), this.random.nextInt(10));
			}
		}
	}
	
	public int getItemInPos(int x, int y) {
		return gridItems.get(new Pair<Integer, Integer>(x, y));
	}
	
	public boolean checkIfExists(int x, int y) {
		return this.gridItems.containsKey(new Pair<Integer, Integer>(x, y));
	}

	public void remove(int x, int y) {
		this.gridItems.remove(new Pair<>(x,y));
	}

	public void propagate(int posX, int posY) {
		for (int x = posX - 1; x <= posX + 1; x++) {
			if (x >= 0 && x < this.gridDimension) {
				for (int y = posY - 1; y <= posY + 1; y++) {
					if (y >= 0 && y < this.gridDimension) {
						this.gridItems.put(new Pair<>(x, y), getItemInPos(posX, posY));
					}
				}
			}
		}
	}

	public void setAbsentCell(int posX, int posY) {
		int sum = 0;
		for (int x = posX - 1; x <= posX + 1; x++) {
			if (x >= 0 && x < this.gridDimension) {
				for (int y = posY - 1; y <= posY + 1; y++) {
					if (y >= 0 && y < this.gridDimension) {
						if (checkIfExists(x, y)) {
							sum += getItemInPos(x, y);
						}
					}
				}
			}
		}
		this.gridItems.put(new Pair<>(posX, posY), sum);
	}

	public void removeNeighbors(int posX, int posY) {
		for (int x = posX - 1; x <= posX + 1; x++) {
			if (x >= 0 && x < this.gridDimension) {
				for (int y = posY - 1; y <= posY + 1; y++) {
					if (y >= 0 && y < this.gridDimension) {
						if (x != posX || y != posY) {
							remove(x, y);
						}
					}
				}
			}
		}
	}

}
