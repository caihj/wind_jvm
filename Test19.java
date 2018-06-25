

public class Test19{

    static {
        System.loadLibrary("test19");
    }

    public static native String   concat(String s1,String s2);

    public static void main(String [] args){

        System.out.println("hello");
    }
}
