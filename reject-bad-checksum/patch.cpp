
struct State {
};

{{ GENERATED_CODE }}

void evaluate(Context ctx) {
    XString str = getValue<input_IN>(ctx);

    auto it = str.iterate();
    if (*it != '$') {
        // Should start with $ and the $ is not accounted in
        // the checksum
        return;
    }

    ++it;

    uint8_t checksum = 0;
    while (it) {
        char c = *it;
        if (c == '*')
            break;

        checksum ^= c;
        ++it;
    }

    if (*it != '*') {
        // Sudden end of line, the command is malformed
        return;
    }

    char checksumStr[3];
    checksumStr[0] = *(++it);
    checksumStr[1] = *(++it);
    checksumStr[2] = '\0';
    uint8_t expectedChecksum = strtoul(checksumStr, nullptr, 16);

    if (expectedChecksum == checksum)
        emitValue<output_OUT>(ctx, str);
}
