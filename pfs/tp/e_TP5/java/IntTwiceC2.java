/*
 * une classe IntTwiceC2, alternative à la classe précédente IntTwiceC1, mais fournissant une
 * méthode statique intTwiceV2F dont l’argument est une fonction f1 de type int → int et
 * le résultat est la fonction réalisant x 7→ f1(f1(x)), avec x de type int 
 */

public class IntTwiceC2 {

    @FunctionalInterface
    interface IntFunction {
        int applyFn(int x);

        default public int applyConstantFn() {
            return 1;
        }
    }

    public static IntFunction intTwiceV2F(IntFunction f1) {
        return (int x) -> f1.applyFn(f1.applyFn(x));
    }

    public static void main(String[] args) {
        IntFunction intTwice = (int n) -> n * 2;
        IntFunction intTwiceV2 = intTwiceV2F(intTwice);
        int result = intTwiceV2.applyFn(2017);
        System.out.println(result);
    }

}
