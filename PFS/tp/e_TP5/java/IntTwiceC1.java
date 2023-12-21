/*
 * une classe IntTwiceC1, fournissant une méthode statique intTwice dont les deux arguments
 * sont une fonction f1 de type int → int et un entier x, le résultat de intTwice est
 * de type int et est égal à f1(f1(x));
 */

public class IntTwiceC1 {

    @FunctionalInterface
    interface IntFunction {
        int applyFn(int x);

        default public int applyConstantFn() {
            return 1;
        }
    }

    public static void main(String[] args) {
        IntFunction intTwice = (int n) -> n * 2;
        int result1 = intTwice.applyFn(2017);
        int result = intTwice.applyFn(result1);
        System.out.println(result1);
        System.out.println(result);

    }

}
