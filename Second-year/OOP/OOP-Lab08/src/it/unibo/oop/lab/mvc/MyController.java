package it.unibo.oop.lab.mvc;

import java.util.LinkedList;
import java.util.List;
import java.util.Objects;

public class MyController implements Controller {
    
    private String nextText;
    private List<String> printedStrings = new LinkedList<>();

    @Override
    public void setNextText(String text) {
        this.nextText = Objects.requireNonNull(this.nextText, "Null value is not accepted");
    }

    @Override
    public String getNextText() {
        return this.nextText;
    }

    @Override
    public List<String> getPrintedStringList() {
        return this.printedStrings;
    }

    @Override
    public void printCurrentString() {
        if (this.nextText == null || this.nextText == "") {
            throw new IllegalStateException("nextText is not set");
        }
        
        this.printedStrings.add(this.nextText);
        System.out.println(this.nextText);
    }
}
