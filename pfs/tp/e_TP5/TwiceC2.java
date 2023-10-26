import java.util.function.Function;

public class TwiceC2 {
    public static <T> Function<T, T> twiceV2F(Function<T, T> f1) {
        return (x) -> f1.apply(f1.apply(x));
    }

    public static void main(String[] args) {
        Function<Integer, Integer> addOne = (x) -> x + 1;
        Function<Integer, Integer> addOneTwice = twiceV2F(addOne);
        System.out.println(addOneTwice.apply(2023));  // Affiche 2025
    }
}
