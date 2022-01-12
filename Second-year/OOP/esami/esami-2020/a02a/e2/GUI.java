package e2;

import javax.swing.*;
import java.util.*;
import java.util.List;
import java.awt.*;
import java.awt.event.ActionListener;

public class GUI extends JFrame {
    
    private static final long serialVersionUID = -6218820567019985015L;
    private final List<JButton> cells = new ArrayList<>();
    private final JButton next = new JButton(">");
    Logic game;
    
    public GUI(int size) {
        this.setDefaultCloseOperation(EXIT_ON_CLOSE);
        this.setSize(100*size, 100*size);
        
        JPanel grid = new JPanel(new GridLayout(size,size));
        this.getContentPane().add(BorderLayout.CENTER,grid);
        this.getContentPane().add(BorderLayout.SOUTH,next);
        
        game = new Logic(size, size);
               
        next.addActionListener(e -> {
        	if (this.game.isGameOver()) {
        		this.next.setEnabled(false);
        	}
        	
        	System.out.println("NEXT");
        	this.game.movePlayer();
        	
        	update();
        });
                
        for (int i=0; i<size; i++){
            for (int j=0; j<size; j++){
                final JButton jb = new JButton(" ");
                this.cells.add(jb);
                grid.add(jb);
            }
        }
        
        update();
        this.setVisible(true);
    }
    
    private void update() {
    	clearBoard();
    	showPlayer();
    	showEnemies();
    	
    }
    
    private void clearBoard() {
    	for (JButton cell : this.cells) {
    		cell.setText(" ");
    	}
    }
    
    private void showEnemies() {
		Iterator<Pair<Integer, Integer>> enemies = this.game.getEnemiesPos();
		while (enemies.hasNext()) {
			Pair<Integer, Integer> enemy = enemies.next();
			
			int x = enemy.getX();
			int y = enemy.getY() * this.game.getGridDimensions().getY();
			this.cells.get(x + y).setText("*");
		}
		
	}

	private void showPlayer() {
		int x = this.game.getPlayerPos().getX();
    	int y = this.game.getPlayerPos().getY() * this.game.getGridDimensions().getY();
    	this.cells.get(x + y).setText("p");
    }
}
