
struct State {
};

{{ GENERATED_CODE }}

void evaluate(Context ctx) {
    XString str = getValue<input_IN>(ctx);
    // TODO: actually check
    emitValue<output_OUT>(ctx, str);
}
