
struct State {
};

{{ GENERATED_CODE }}

void evaluate(Context ctx) {
    Serial.begin(115200);

    XString str = getValue<input_IN>(ctx);
    for (auto it = str.iterate(); it; ++it)
        Serial.write(*it);

    Serial.println("");
}
