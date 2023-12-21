import java.util.function.Function;

public class TwiceC1 {
    public static <T> T twiceV2(Function<T, T> f1, T x) {
        return f1.apply(f1.apply(x));
    }

    public static void main(String[] args) {
        Function<Integer, Integer> addOne = (x) -> x + 1;
        System.out.println(twiceV2(addOne, 2023));  // Affiche 2025
    }
}
