package a01c.e2;

import javax.swing.*;
import java.util.*;
import java.util.List;
import java.awt.*;
import java.awt.event.ActionListener;

public class GUIExample extends JFrame {
    
    private static final long serialVersionUID = -6218820567019985015L;
    private final List<JButton> cells = new ArrayList<>();
    private Logicimpl game;
    private int size;
    
    public GUIExample(int size) {
    	this.size = size;
        this.setDefaultCloseOperation(EXIT_ON_CLOSE);
        this.setSize(50*size, 50*size);
        
        JPanel panel = new JPanel(new GridLayout(size,size));
        this.getContentPane().add(panel);
        
        game = new Logicimpl(size);
        
        ActionListener al = e -> {
        	var button = (JButton)e.getSource();
        	int y = cells.indexOf(button)/this.size;
        	int x = this.size*this.size - y*this.size;
        			
        	this.game.clicked(x, y);
        	update();
        	//button.setEnabled(false); 
        };
                
        for (int i=0; i<size; i++){
            for (int j=0; j<size; j++){
                final JButton jb = new JButton(" ");
                this.cells.add(jb);
                jb.addActionListener(al);
                panel.add(jb);
            }
        }
        this.setVisible(true);
    }
    
    private void update() {
    	for (int i=0; i<this.size; i++){
            for (int j=0; j<this.size; j++){
            	int coord = i*this.size+j;
            	JButton btn = this.cells.get(coord);
            	
            	if (this.game.wasSigned(j, i)) {
            		System.out.println("f");
            		btn.setText("*");
            	}
            }
        }
    }
}
