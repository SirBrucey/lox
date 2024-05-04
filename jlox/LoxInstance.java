package jlox;

public class LoxInstance {
    private LoxClass cls;

    LoxInstance(LoxClass cls) {
        this.cls = cls;
    }

    @Override
    public String toString() {
        return cls.name + " instance";
    }
}
