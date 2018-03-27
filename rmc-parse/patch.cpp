
struct State {
    char time[16];
    CStringView timeView;

    State() : timeView(time) {};
};

constexpr Number KNOTS_TO_METERS_PER_SECOND = 0.514444;

{{ GENERATED_CODE }}

void takeToken(Iterator<char>& it, char* buff) {
    char c;
    while (it) {
        c = *it;
        if (c == ',' || c == '*') {
            ++it; // skip past a delimiter
            break;
        }

        *buff++ = c;
        ++it;
    }

    *buff = '\0'; // enforce terminating null
}

void evaluate(Context ctx) {
    /*
      From http://www.trimble.com/oem_receiverhelp/v4.44/en/NMEA-0183messages_RMC.html

      The RMC string is:

      $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A

      0	Message ID $GPRMC
      1	UTC of position fix
      2	Status A=active or V=void
      3	Latitude
      4	Longitude
      5	Speed over the ground in knots
      6	Track angle in degrees (True)
      7	Date
      8	Magnetic variation in degrees
      9	The checksum data, always begins with *
    */
    Number x;
    char buff[16] = { 0 };
    State* state = getState(ctx);

    XString str = getValue<input_IN>(ctx);
    auto it = str.iterate();

    // Expect $GxRMC or $GxRMC
    takeToken(it, buff);
    if (buff[0] != '$' || strncmp(buff + 3, "RMC", 3) != 0) {
        // Nope, it’s not that message
        emitValue<output_REJ>(ctx, str);
        return;
    }

    // UTC
    takeToken(it, state->time);
    emitValue<output_TIME>(ctx, XString(&state->timeView));

    // Status
    takeToken(it, buff);
    emitValue<output_ACT>(ctx, buff[0] == 'A');

    // Latitude
    takeToken(it, buff);
    x = atof(buff);
    takeToken(it, buff);
    if (buff[0] == 'S') {
        // South is negative
        x = -x;
    }

    emitValue<output_LAT>(ctx, x);

    // Longitude
    takeToken(it, buff);
    x = atof(buff);
    takeToken(it, buff);
    if (buff[0] == 'W') {
        // West is negative
        x = -x;
    }

    emitValue<output_LON>(ctx, x);

    // Speed
    takeToken(it, buff);
    x = atof(buff) * KNOTS_TO_METERS_PER_SECOND;
    emitValue<output_SPD>(ctx, x);

    // Course
    takeToken(it, buff);
    x = atof(buff);
    emitValue<output_CRS>(ctx, x);
}
