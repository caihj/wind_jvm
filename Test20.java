class P{
    public void foo(){
        System.out.println("p:foo()");
    }

    public static void staticfoo(){

    }
}

class PA extends P{
    @Override
    public void foo() {
        System.out.println("PA:foo()");
    }
}

class PB extends P{
    @Override
    public void foo() {
        super.foo();
    }
}

public class Test20 {

    public static void main(String[] args) {
        PA.staticfoo();
        P.staticfoo();
        P p = new PA();
        p.foo();

        p=new PB();
        p.foo();
    }
}
