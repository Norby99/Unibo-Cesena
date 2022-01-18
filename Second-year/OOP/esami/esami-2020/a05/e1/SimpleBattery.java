package e1;

public class SimpleBattery extends AbstractBatteryImpl {
	
	protected double energyDropXmin;
	
	public SimpleBattery(final double energyDropXmin) {
		super();
		this.energyDropXmin = energyDropXmin;
	}
	
	@Override
	public void stopUse(final double duration) {
		super.stopUse(duration);
		setCurrentEnergy(this.getEnergy() - this.energyDropXmin * duration);
	}

	@Override
	public void recharge() {
		
	}

}
