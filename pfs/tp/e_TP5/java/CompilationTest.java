public class CompilationTest {
    // permet de construire des fonctions à partir d'expressions lambda
    @FunctionalInterface
    interface IntFunction {
        int applyFn(int x);

        default public int applyConstantFn() { // méthode par défaut
            return 1;
        }
    }

    public static void main(String[] args) {
        final int factor = 2;
        IntFunction f = (int n) -> n * factor; // crée une fonction n*factor
        IntFunction f0 = (int n) -> 2 * n * factor; // crée une fonction 2*n*factor
        System.out.println(f.applyFn(2017));
        System.out.println(f0.applyFn(2017));
        Runnable ending = () -> System.out.println("That's the end, my fellas!");
        ending.run();
    }
}
