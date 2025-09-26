package bbom.model;

public class IncreasableState implements State {
    private int state;

    public IncreasableState() {
        this.state = 0;
    }

    @Override
    public void update() {
        state++;
    }

    @Override
    public String toString() {
        return "State value: " + state;
    }
}
