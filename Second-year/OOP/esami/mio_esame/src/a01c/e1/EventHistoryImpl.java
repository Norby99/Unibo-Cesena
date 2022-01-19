package a01c.e1;

import java.util.HashMap;
import java.util.Map;

public class EventHistoryImpl<E> implements EventHistory<E>{
	
	private Map<Double, E> map = new HashMap<Double, E>();
	private Double currentEvent; 

	public EventHistoryImpl(Map<Double, E> map) {
		this.map = map;
		this.currentEvent = map.keySet().iterator().next();
		setFirstEvent();
	}
	
	private void setFirstEvent() {
		for (double t : this.map.keySet()) {
			if (this.currentEvent > t) {
				this.currentEvent = t;
			}
		}
	}
	
	private double findLast() {
		double i = this.currentEvent;
		for (double t : this.map.keySet()) {
			if (i < t) {
				i = t;
			}
		}
		return i;
	}
	
	@Override
	public double getTimeOfEvent() {
		return this.currentEvent;
	}
	
	@Override
	public E getEventContent() {
		return this.map.get(this.currentEvent);
	}

	@Override
	public boolean moveToNextEvent() {
		boolean found = false;
		double tempEvT = findLast();
		
		for (double t : this.map.keySet()) {
			if (tempEvT >= t && t > this.currentEvent) {
				tempEvT = t;
				found = true;
			}
		}
		
		if (found) {
			this.currentEvent = tempEvT;
		}
		return found;
	}

}
