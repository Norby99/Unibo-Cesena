package e1;

import java.util.Map;
import java.util.Optional;
import java.util.Set;

public class GroupingImpl<G,V> implements Grouping<G,V> {
	
	private Map<G,Set<V>> map;
	
	public GroupingImpl(Map<G,Set<V>> m) {
		this.map = m;
	}

	@Override
	public Set<V> getValuesOfGroup(G group) {
		return this.map.get(group);
	}

	@Override
	public Set<G> getGroups() {
		return this.map.keySet();
	}
	
	@Override
	public Optional<G> getGroupOf(V data) {
		Optional<G> val = Optional.empty();
		for (G key : this.map.keySet()) {
			if (map.get(key).contains(data)) {
				val =  Optional.of(key);
			}
		}
		return val;
	}

	@Override
	public Map<G,Set<V>> asMap() {
		return this.map;
	}

	@Override
	public Grouping<G,V> combineGroups(G initial1, G initial2, G result) {
		// TODO Auto-generated method stub
		return null;
	}

}
