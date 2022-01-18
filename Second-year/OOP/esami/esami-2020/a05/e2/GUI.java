package e2;

import javax.swing.*;
import java.util.*;
import java.util.List;
import java.awt.*;
import java.awt.event.ActionListener;

public class GUI extends JFrame {
    
    private static final long serialVersionUID = -6218820567019985015L;
    private final List<JButton> cells = new ArrayList<>();
    private final Logic grid;
    private final int size;
    
    public GUI(int size) {
    	this.size = size;
        this.setDefaultCloseOperation(EXIT_ON_CLOSE);
        this.setSize(100*size, 100*size);
        
        JPanel panel = new JPanel(new GridLayout(size,size));
        this.getContentPane().add(panel);
        
        grid = new Logic(size);
        
        ActionListener al = e -> {
        	var button = (JButton)e.getSource();
        	
        	var position = cells.indexOf(button);
        	int posY = position/size;
        	int posX = position - posY * size;
        	
        	if (grid.checkIfExists(posX, posY)) {
	        	grid.propagate(posX, posY);
	        	grid.remove(posX, posY);
        	} else {
        		grid.setAbsentCell(posX, posY);
	        	grid.removeNeighbors(posX, posY);
        	}
        	
        	update();
        };
                
        for (int i=0; i<size; i++){
            for (int j=0; j<size; j++){
                final JButton jb = new JButton(String.valueOf(grid.getItemInPos(i, j)));
                this.cells.add(jb);
                jb.addActionListener(al);
                panel.add(jb);
            }
        }
        this.setVisible(true);
    }
    
	private void update() {
		for (JButton btn: this.cells) {
			var position = cells.indexOf(btn);
        	int posY = position/size;
        	int posX = position - posY * size;
        	
        	if (grid.checkIfExists(posX, posY)) {
        		btn.setText(String.valueOf(grid.getItemInPos(posX, posY)));
        	} else {
        		btn.setText("X");
        	}
        	
        	
		}
	}
    
}
