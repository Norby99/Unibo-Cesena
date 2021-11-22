package it.unibo.oop.lab.reactivegui03;

import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.lang.reflect.InvocationTargetException;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

public final class AnotherConcurrentGUI extends JFrame {

    private static final long serialVersionUID = 1L;
    private static final double WIDTH_PERC = 0.2;
    private static final double HEIGHT_PERC = 0.1;
    private final JLabel display = new JLabel();
    private final JButton btDown = new JButton("down");
    private final JButton btUp = new JButton("up");
    private final JButton stop = new JButton("stop");

    /**
     * Builds a new CGUI.
     */
    public AnotherConcurrentGUI() {
        super();
        this.setTitle("Reactivegui03");
        final Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        this.setSize((int) (screenSize.getWidth() * WIDTH_PERC), (int) (screenSize.getHeight() * HEIGHT_PERC));
        this.setDefaultCloseOperation(EXIT_ON_CLOSE);
        final JPanel panel = new JPanel();
        final JPanel buttonCanvas = new JPanel();
        buttonCanvas.setLayout(new BoxLayout(buttonCanvas, BoxLayout.X_AXIS));
        buttonCanvas.add(btUp);
        buttonCanvas.add(btDown);
        buttonCanvas.add(stop);
        panel.add(display);
        panel.add(buttonCanvas);
        this.getContentPane().add(panel);
        this.setVisible(true);

        final Agent agent = new Agent();
        new Thread(agent).start();

        stop.addActionListener(new ActionListener() {

            @Override
            public void actionPerformed(final ActionEvent e) {
                agent.manageCounting();
            }
        });
        
        btUp.addActionListener(new ActionListener() {
            
            @Override
            public void actionPerformed(final ActionEvent e) {
                agent.setUp();
            }
        });
        
        btDown.addActionListener(new ActionListener() {
            
            @Override
            public void actionPerformed(final ActionEvent e) {
                agent.setDown();
            }
        });
        
    }

    /*
     * The counter agent is implemented as a nested class. This makes it
     * invisible outside and encapsulated.
     */
    private class Agent implements Runnable {

        final TimerAgent timeAgent = new TimerAgent();
        private volatile boolean stop;
        private int counter;
        private boolean directionUp = true; // true if counter is incrementing, false if it is decrementing

        @Override
        public void run() {
            while (true) {    
                try {
                    if (this.isCounting() && this.isValid()) {
                        /*
                         * All the operations on the GUI must be performed by the
                         * Event-Dispatch Thread (EDT)!
                         */
                        final var curr = Integer.toString(this.counter);
                        SwingUtilities.invokeAndWait(new Runnable() {
                            @Override
                            public void run() {
                                // This will happen in the EDT: since i'm reading counter it needs to be volatile.
                                AnotherConcurrentGUI.this.display.setText(curr);
                            }
                        });
                        this.counter += intValOfDirection();
                    }
                    Thread.sleep(100);
                } catch (InvocationTargetException | InterruptedException ex) {
                    ex.printStackTrace();
                }
            }
        }

        /**
         * External command to stop counting.
         */
        public void stopCounting() {
            this.stop = true;
            AnotherConcurrentGUI.this.stop.setText("start");
        }
        
        public boolean isValid() {
            return this.timeAgent.isTimeOver();
        }
        
        public void startCounting() {
            this.stop = false;
            AnotherConcurrentGUI.this.stop.setText("stop");
        }
        
        public boolean isCounting() {
            return !this.stop;
        }
        
        private int intValOfDirection() {
            if (this.directionUp) {
                return 1;
            } else {
                return -1;
            }
        }
        
        public void setUp() {
            this.directionUp = true;
        }
        
        public void setDown() {
            this.directionUp = false;
        }
        
        public void manageCounting() {
            if (this.isCounting()) {
                this.stopCounting();
            } else {
                this.startCounting();
            }
        }
    }
    
    private class TimerAgent implements Runnable {
        
        private final int timeLimit = 1000;
        private boolean endTime;

        @Override
        public void run() {
            try {
                Thread.sleep(this.timeLimit);
                this.setTimeOver();
            } catch (InterruptedException ex) {
                ex.printStackTrace();
            }
        }
        
        private void setTimeOver() {
            this.endTime = true;
        }
        
        public volatile boolean isTimeOver() {
            return !this.endTime;
        }
    }
}