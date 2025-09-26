package bbom.view;

import bbom.controller.input.EventType;
import bbom.controller.input.InputListener;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class MyAppView extends JFrame  implements ActionListener {

    private final InputListener inputListener;

    public MyAppView(InputListener inputListener) {
        super("My BBoM App");

        this.inputListener = inputListener;

        setSize(300, 70);
        setResizable(false);

        var updateButton = new JButton("Update");
        updateButton.addActionListener(this);

        var panel = new JPanel();
        panel.add(updateButton);

        setLayout(new BorderLayout());
        add(panel,BorderLayout.NORTH);

        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent ev) {
                System.exit(-1);
            }
        });
    }

    public void actionPerformed(ActionEvent ev) {
        inputListener.notify(EventType.INCREASE);
    }
}
