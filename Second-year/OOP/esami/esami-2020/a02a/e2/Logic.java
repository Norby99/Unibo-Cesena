package e2;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

public class Logic {

	private Pair<Integer, Integer> gridDimention;
	final Random randomGenerator = new Random();
	private Pair<Integer, Integer> playerPosition = new Pair<>();
	private List<Pair<Integer, Integer>> enemiesPositions = new LinkedList<Pair<Integer, Integer>>();
	
	public Logic(final int x, final int y) {
		gridDimention = new Pair<>(x, y);
	
		setPlayerPos(this.randomGenerator.nextInt(x), this.gridDimention.getY() - 1);
		setStarsPosition(x);
		
	}
	
	public void movePlayer() {
		if (isEnemyOnTopLeft()) {
			this.playerPosition.setX(this.playerPosition.getX()-1);
			this.playerPosition.setY(this.playerPosition.getY()-1);
		} else if (isEnemyOnTopRight()) {
			this.playerPosition.setX(this.playerPosition.getX()+1);
			this.playerPosition.setY(this.playerPosition.getY()-1);
		} else {
			if (!isEndBoard(this.playerPosition.getY()) && isNextCellFree()) {
				this.playerPosition.setY(this.playerPosition.getY()-1);
			}
		}
	}

	private boolean isEndBoard(Integer y) {
		return y == 0;
	}

	private boolean isNextCellFree() {
		Boolean validPos = true;
		Pair<Integer, Integer> nextPos = new Pair<>(this.playerPosition.getX(), this.playerPosition.getY()-1);
		for (Pair<Integer, Integer> enemy : this.enemiesPositions) {
			if (enemy.equals(nextPos)) {
				validPos = false;
			}
		}
		return validPos;
	}

	private boolean isEnemyOnTopRight() {
		return isInPosition(1, -1);
	}

	private boolean isEnemyOnTopLeft() {
		return isInPosition(-1, -1);
	}
	
	private boolean isInPosition(Integer x, Integer y) {
		Boolean spotted = false;
		Pair<Integer, Integer> nextPos = new Pair<>(this.playerPosition.getX() + x, this.playerPosition.getY() + y);
		for (Pair<Integer, Integer> enemy : this.enemiesPositions) {
			if (enemy.equals(nextPos)) {
				this.enemiesPositions.remove(enemy);
				spotted = true;
				break;
			}
		}
		
		return spotted;
	}

	private void setStarsPosition(int x) {
		while (this.enemiesPositions.size() < x) {
			Pair<Integer, Integer> pos = new Pair<>(this.randomGenerator.nextInt(this.gridDimention.getX()), this.randomGenerator.nextInt(this.gridDimention.getY()-1));
			Boolean validPos = true;
			for (Pair<Integer, Integer> enemy : this.enemiesPositions) {
				if (enemy.equals(pos)) {
					validPos = false;
				}
			}
			if (validPos) {
				this.enemiesPositions.add(pos);
			}
		}
	}
	
	private void setPlayerPos(int x, int y) {
		this.playerPosition.setX(x);
		this.playerPosition.setY(y);
	}
	
	public Pair<Integer, Integer> getPlayerPos() {
		return this.playerPosition;
	}
	
	public Iterator<Pair<Integer, Integer>> getEnemiesPos() {
		return this.enemiesPositions.iterator();
	}
	
	public Pair<Integer, Integer> getGridDimensions() {
		return this.gridDimention;
	}
	
	public Boolean isGameOver() {
		return isEndBoard(this.playerPosition.getY()) || !isNextCellFree();
	}
	
}
