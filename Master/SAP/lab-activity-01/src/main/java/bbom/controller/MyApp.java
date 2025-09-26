package bbom.controller;

import bbom.controller.input.InputListener;
import bbom.utils.Logger;
import bbom.model.State;
import bbom.model.IncreasableState;
import bbom.controller.input.EventType;
import bbom.view.MyAppView;

public class MyApp implements InputListener {

	private final State state;
	private final MyAppView view;
	
	public MyApp() {
		view = new MyAppView(this);

		state = new IncreasableState();
		Logger.log(state.toString());
	}

	public void initialize() {
		view.setVisible(true);
	}

	public void notify(EventType event) {
		switch (event) {
			case INCREASE -> update();
		}
	}

	private void update() {
		state.update();
		Logger.log(state.toString());
	}
}
