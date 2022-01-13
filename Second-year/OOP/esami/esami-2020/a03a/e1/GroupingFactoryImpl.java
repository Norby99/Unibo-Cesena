package e1;

import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.function.BiPredicate;
import java.util.function.Function;
import java.util.function.Predicate;

public class GroupingFactoryImpl implements GroupingFactory {

	@Override
	public <G, V> Grouping<G, V> fromPairs(Iterable<Pair<G, V>> values) {
		Map<G,Set<V>> map = new HashMap<>();
		for (Pair<G, V> el : values) {
			if (map.containsKey(el.getX())) {
				Set<V> tempSet = map.get(el.getX());
				tempSet.add(el.getY());
				map.put(el.getX(), tempSet);
			} else {
				Set<V> tempSet = new HashSet<>();
				tempSet.add(el.getY());
				map.put(el.getX(), tempSet);
			}
		}
		return new GroupingImpl<>(map);
	}

	@Override
	public <V> Grouping<V, V> singletons(Set<V> values) {
		List<Pair<V, V>> iter = new LinkedList<>();
		for (V val : values) {
			iter.add(new Pair<>(val, val));
		}
		return fromPairs(iter);
	}

	@Override
	public <V> Grouping<V, V> withChampion(Set<V> values, BiPredicate<V, V> sameGroup, Predicate<V> champion) {
		List<Pair<V, V>> iter = new LinkedList<>();
		
		for (V i : values) {
			Boolean inserted = false;
			for (Pair<V, V> p : iter) {
				V j = p.getX();
				if (sameGroup.test(i, j)) {
					iter.add(new Pair<>(j, i));
					inserted = true;
					break;
				}
			}
			if (!inserted) {
				iter.add(new Pair<>(i, i));
			}
		}
		return fromPairs(iter);
	}

	@Override
	public <G, V> Grouping<G, V> fromFunction(Set<V> values, Function<V, G> mapper) {
		// TODO Auto-generated method stub
		return null;
	}

}
