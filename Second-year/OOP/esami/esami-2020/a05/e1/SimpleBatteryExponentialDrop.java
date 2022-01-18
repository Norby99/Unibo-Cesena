package e1;

public class SimpleBatteryExponentialDrop extends AbstractBatteryImpl {
	
	public SimpleBatteryExponentialDrop() {
		super();
	}
	
	public void startUse() {
		super.startUse();
		setCurrentEnergy(getEnergy()/2);
	}

	@Override
	public void recharge() {

	}

}
