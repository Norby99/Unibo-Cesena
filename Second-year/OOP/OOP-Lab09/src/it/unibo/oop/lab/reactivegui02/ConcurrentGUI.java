package it.unibo.oop.lab.reactivegui02;

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

public final class ConcurrentGUI extends JFrame {

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
    public ConcurrentGUI() {
        super();
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
        /*
         * Create the counter agent and start it. This is actually not so good:
         * thread management should be left to
         * java.util.concurrent.ExecutorService
         */
        final Agent agent = new Agent();
        new Thread(agent).start();
        /*
         * Register a listener that stops it
         */
        stop.addActionListener(new ActionListener() {
            /**
             * event handler associated to action event on button stop.
             * 
             * @param e
             *            the action event that will be handled by this listener
             */
            @Override
            public void actionPerformed(final ActionEvent e) {
                // Agent should be final
                agent.manageCounting();
            }
        });
    }

    /*
     * The counter agent is implemented as a nested class. This makes it
     * invisible outside and encapsulated.
     */
    private class Agent implements Runnable {

        private volatile boolean stop;
        private volatile int counter;
        private boolean directionUp; // true if counter is incrementing, false if it is decrementing

        @Override
        public void run() {
            while (true) {
                if(isCounting()) {
                    try {
                        /*
                         * All the operations on the GUI must be performed by the
                         * Event-Dispatch Thread (EDT)!
                         */
                        SwingUtilities.invokeAndWait(new Runnable() {
                            @Override
                            public void run() {
                                // This will happen in the EDT: since i'm reading counter it needs to be volatile.
                                ConcurrentGUI.this.display.setText(Integer.toString(Agent.this.counter));
                            }
                        });
                        this.counter++;
                        Thread.sleep(100);
                    } catch (InvocationTargetException | InterruptedException ex) {
                        ex.printStackTrace();
                    }
                }
            }
        }

        /**
         * External command to stop counting.
         */
        public void stopCounting() {
            this.stop = true;
            ConcurrentGUI.this.stop.setText("start");
        }
        
        public void startCounting() {
            this.stop = false;
            ConcurrentGUI.this.stop.setText("stop");
        }
        
        public boolean isCounting() {
            return !this.stop;
        }
        
        public void manageCounting() {
            if(this.isCounting()){
                this.stopCounting();
            }else {
                this.startCounting();
            }
        }
    }
}