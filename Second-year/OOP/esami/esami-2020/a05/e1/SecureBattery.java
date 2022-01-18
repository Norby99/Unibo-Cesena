package e1;

public class SecureBattery extends SimpleBattery {

	public SecureBattery(double energyDropXmin) {
		super(energyDropXmin);
	}
	
	@Override
	public void startUse() {
		if (!isUsed()) {
			if (this.getEnergy() > 0) {
				super.startUse();
			} else {
				throw new IllegalStateException("Battery can not go lower than 0%");
			}
		} else {
			throw new IllegalStateException("Battery already in use");
		}
	}
	
	@Override
	public void stopUse(final double duration) {
		if (isUsed()) {
			super.stopUse(duration);
		} else {
			throw new IllegalStateException("Battery not in use");
		}
	}

}
