#include "receive.h"

/**
 * @brief Handles the transmission of a packet and updates the motor data.
 *
 * This function receives a packet, checks its validity, and parses it to update the motor data.
 * If the transmission is successful, it sets the new_data flag to true.
 *
 * @param motor A pointer to the motor data structure.
 * @return The status of the transmission.
 */
ret_t handle_transmission(packet_t* const motor) // remember to use Serial.peek() to ckeck to run transmission before calling this ig ?
{
    char packet[50] {};
    packet[49] = '\0'; // so this is a valid string
    ret_t ret = recieve_packet(packet);
    if (ret != ret_t::SUCCESS)
        return ret;
    // ret = check_packet(); // TODO
    ret = parse_packet(packet, motor);
    if (ret != ret_t::SUCCESS)
        return ret;

    Serial.print("~~ACK**");
    new_data = true;
    return ret_t::SUCCESS;
}

/** @brief Receives a packet from the serial communication and validates its format.
 *
 * This function reads characters from the serial communication until it receives a complete packet.
 * The packet should start with the '~' character and end with the '*' character.
 * The function stores the received packet in the provided `packet` buffer.
 *
 * @param packet A pointer to the buffer where the received packet will be stored.
 *         - `ret_t::SUCCESS` if the packet is received successfully. \n
 *         - `ret_t::WRONG_FMT` if the packet format is incorrect (missing end byte or wrong end byte). \n
 *         - `ret_t::SPICY` if the last byte of the packet cannot be read. \n
 *         - `ret_t::ERROR` if there is an error in the serial communication. \n
 *         - `ret_t::LITERALLY_IMPOSSIBLE` if the function reaches an unreachable return statement. \n
 */
ret_t recieve_packet(char* const packet)
{
    if (Serial.available()) {
        char c = Serial.read();
        if (c == '~') {
            // we have a packet

            // read the packet
            Serial.readBytesUntil('*', packet, 49); // tbh the length is kinda just a guess lol

            // check the end byte
            if (Serial.available()) {
                char end = Serial.read();
                if (end == '*') {
                    return ret_t::SUCCESS;
                } else {
                    return ret_t::WRONG_FMT1; // no end byte or wrong end byte
                }
            } else {
                return ret_t::SPICY; // cant read last byte???
            }

        } else { // wrong start byte
            return ret_t::WRONG_FMT0;
        }
    } else {
        return ret_t::ERROR;
    }
    // this should never happen but just in case
    return ret_t::LITERALLY_IMPOSSIBLE; // actually, if it does reach this, I'm going to be very impressed
} // it turns out if you're consistent with the return value thing, it gets quite full lol

/**
 * @brief Parses a token and returns its integer value.
 *
 * This function takes a null-terminated string `token` and converts it into an integer value.
 * The string is expected to represent a valid integer. If the string cannot be converted to an integer,
 * the behavior is undefined.
 *
 * @param token The null-terminated string to be parsed.
 * @return The integer value represented by the token.
 */
const uint8_t parse_token(char* const token)
{
    /// strtoul converts a string to an unsigned long int
    /// the third argument is the base of the number system
    /// 10 is for decimal
    /// the middle argument is a pointer to a char pointer
    /// when set to null, it is not used
    const uint8_t val = atoi(token);
    // Serial.print("Parsed token: ");
    // Serial.print(token);
    // Serial.print(" -> ");
    // Serial.println(val);
    return val; // i despise this cast
    // returns 0 if the string is not a valid integer (or if it's 0 lol)
}

/**
 * @brief Tokenizes the input buffer and extracts motor, state, direction, steps, and speed values.
 *
 * @param buffer The input buffer containing the tokens.
 * @param motor Reference to the variable to store the motor value.
 * @param state Reference to the variable to store the state value.
 * @param direction Reference to the variable to store the direction value.
 * @param steps Reference to the variable to store the steps value.
 * @param speed Reference to the variable to store the speed value.
 * @return The result of the tokenization process.
 */
ret_t tokenize(char* const buffer, uint8_t& motor, uint8_t& state, uint8_t& direction, uint8_t& steps, uint8_t& speed)
{
    // ngl this is kinda spaghetti
    char* token = nullptr;
    // Serial.println("buffer: ");
    // Serial.println(buffer);

    /// lambda function to get the next token
    auto get_next_token = [&token](char* const buffer) -> ret_t { // this is a lambda function, it's like a function but it's defined in the scope of another function
        static constexpr char sep[2] = ",";
        token = strtok(buffer, sep);
        if (token != nullptr)
            return ret_t::SUCCESS;
        else
            return ret_t::ERROR;
    };

    // Ok, i'll explain this only once
    // we first get the next token (the individual bits of info in the packet between the commas), if it's not successful, we return the error
    // tbh i'm not sure if we need the functionas a lambda, but strtok is weird and I don't want to mess with it
    // it's caused me enough pain already
    // we pass the buffer to the lambda function
    ret_t ret = get_next_token(buffer);
    if (ret != ret_t::SUCCESS)
        return ret;
    motor = parse_token(token); // we parse the token into a uint8_t intand save it to the motor variable

    ret = get_next_token(nullptr); // because we're using strtok, we pass null to get the next token (it remembers the buffer from the last call)
    if (ret != ret_t::SUCCESS) // the nullptr thing is weird to me too
        return ret;
    state = parse_token(token);

    ret = get_next_token(nullptr); // we do this for all the other tokens
    if (ret != ret_t::SUCCESS)
        return ret;
    direction = parse_token(token);

    ret = get_next_token(nullptr);
    if (ret != ret_t::SUCCESS)
        return ret;
    steps = parse_token(token);

    ret = get_next_token(nullptr);
    if (ret != ret_t::SUCCESS)
        return ret;
    speed = parse_token(token);

    return ret_t::SUCCESS; // if we get here, everything went well
}

ret_t parse_packet(char* const buffer, packet_t* const packet)
{
    uint8_t motor, state, direction, steps, speed;

    ret_t ret = tokenize(buffer, motor, state, direction, steps, speed);
    if (ret != ret_t::SUCCESS)
        return ret; // if the tokenization fails, we return the error

    packet->motor = motor;
    packet->state = state;
    packet->direction = direction;
    packet->steps = steps;
    packet->speed = speed;

    return ret_t::SUCCESS;
}