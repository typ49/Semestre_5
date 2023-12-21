public class Average {

    @FunctionalInterface
    interface FloatFunction {
        float applyFn(float x);

        default public float applyConstantFn() {
            return 1;
        }
    }

    public static FloatFunction makeAverageFunction(float x) {
        return new FloatFunction() {
            @Override
            public float applyFn(float y) {
                return (x + y) / 2;
            }
        };
    }

    public static void main(String[] args) {
        FloatFunction averageWith10 = makeAverageFunction(10.0f);
        System.out.println(averageWith10.applyFn(12.0f));  // Affiche 11.0

        FloatFunction averageWith12 = makeAverageFunction(12.0f);
        System.out.println(averageWith12.applyFn(13.0f));  // Affiche 12.5
    }
}
