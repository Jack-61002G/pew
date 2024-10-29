#include "lights.hpp"

struct RGB {
    uint8_t r, g, b;
    RGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};

int hexToDecimal(const std::string& hex) {
    return std::stoi(hex, nullptr, 16);
}

RGB hexToRGB(const std::string& hex) {
    std::string cleanHex = hex;
    if (cleanHex[0] == '#') {
        cleanHex = cleanHex.substr(1);
    }
    
    int r = hexToDecimal(cleanHex.substr(0, 2));
    int g = hexToDecimal(cleanHex.substr(2, 2));
    int b = hexToDecimal(cleanHex.substr(4, 2));
    
    return RGB(r, g, b);
}

std::vector<RGB> interpolateColors(const std::string& hex1, const std::string& hex2, int stripLength) {
    RGB color1 = hexToRGB(hex1);
    RGB color2 = hexToRGB(hex2);
    
    std::vector<RGB> result;
    result.reserve(stripLength);
    
    if (stripLength == 1) {
        result.push_back(color1);
        return result;
    }
    
    double stepR = static_cast<double>(color2.r - color1.r) / (stripLength - 1);
    double stepG = static_cast<double>(color2.g - color1.g) / (stripLength - 1);
    double stepB = static_cast<double>(color2.b - color1.b) / (stripLength - 1);
    
    for (int i = 0; i < stripLength; ++i) {
        int r = static_cast<int>(color1.r + stepR * i);
        int g = static_cast<int>(color1.g + stepG * i);
        int b = static_cast<int>(color1.b + stepB * i);
        
        r = std::clamp(r, 0, 255);
        g = std::clamp(g, 0, 255);
        b = std::clamp(b, 0, 255);
        
        result.emplace_back(r, g, b);
    }
    
    return result;
}

void lib::Lights::loop() {
    int currentTeam = 3;
    int currentIndicator = -1;
    int currentWarning = 0;
    int offset = 0;
    
    // Create initial gradient for the LED strip
    std::vector<RGB> gradient = interpolateColors("#FF0000", "#0000FF", 40);
    
    // Pre-calculate all color integers for the strip
    std::vector<uint32_t> stripColors(40);
    for (int i = 0; i < 40; i++) {
        RGB color = gradient[i];
        stripColors[i] = (static_cast<uint32_t>(color.r) << 16) | 
                        (static_cast<uint32_t>(color.g) << 8) | 
                        static_cast<uint32_t>(color.b);
    }
    
    while (true) {
            if (startTime != -1) {
                int dT = pros::millis() - startTime;
                
                if (5000 < dT && dT < 5500) {
                    if (currentWarning != 1) {
                        currentWarning = 1;
                        leftDriveLed.set_all(warning_1);
                        pros::delay(20);
                        rightDriveLed.set_all(warning_1);
                        pros::delay(20);
                    }
                }
                else if (10000 < dT && dT < 10500) {
                    if (currentWarning != 2) {
                        currentWarning = 2;
                        leftDriveLed.set_all(warning_2);
                        pros::delay(20);
                        rightDriveLed.set_all(warning_2);
                        pros::delay(20);
                    }
                }
                else if (currentWarning != 0) {
                    currentWarning = 0;
                    currentTeam = -1;
                }
            }

            // Update indicator lights
            if (indicator != currentIndicator) {
                currentIndicator = indicator;
                if (indicator == 1) {
                    indicatorLed1.set_all(team == 1 ? red : blue);
                    pros::delay(20);
                    indicatorLed2.set_all(team == 1 ? red : blue);
                    pros::delay(20);
                } else {
                    indicatorLed1.clear();
                    pros::delay(20);
                    indicatorLed2.clear();
                    pros::delay(20);
                }
            }
        }

        // Update team colors if changed
        if (team != currentTeam && currentWarning == 0) {
            currentTeam = team;
            if (team == 1) {
                gradient = interpolateColors("#ff0000", "#e63410", 40);
            } else if (team == 2) {
                gradient = interpolateColors("#003080", "#41b5ff", 40);
            } else if (team == 3) {
                gradient = interpolateColors("#FF0000", "#FF00FF", 40);
            } else {
                gradient = interpolateColors("#620062", "#902090", 40);
            }
            
            // Update the pre-calculated colors
            for (int i = 0; i < 40; i++) {
                RGB color = gradient[i];
                stripColors[i] = (static_cast<uint32_t>(color.r) << 16) | 
                                (static_cast<uint32_t>(color.g) << 8) | 
                                static_cast<uint32_t>(color.b);
            }
        }

        // Update LED strips with flowing gradient
        if (currentWarning == 0) {
            // Update all LEDs on left strip
            for (int i = 0; i < 40; i++) {
                int colorIndex = (i + offset) % 40;
                leftDriveLed.set_pixel(stripColors[colorIndex], i);
            }
            pros::delay(20);  // Required delay between strips
            
            // Update all LEDs on right strip
            for (int i = 0; i < 40; i++) {
                int colorIndex = (i + offset) % 40;
                rightDriveLed.set_pixel(stripColors[colorIndex], i);
            }
            
            // Increment offset for next update
            offset = (offset + 1) % 40;
            pros::delay(20);  // Delay before next update
        }
    }