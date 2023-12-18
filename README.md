# C-McourseProject
**Measurements and Circuits course project ,Dept. of Electrical and Electronics Engineering ,MITS** 


**Project Overview: Energy Meter with Buzzer and LCD Display**

**1. Introduction:**
   The project aims to create an energy meter with additional features such as a piezo buzzer for audio feedback and an LCD display for visual output. It is designed to measure voltage, current, power, ampere-hours, and energy consumption. The system includes a relay for controlling power based on current limits and a button for manual control.

**2. Components Used:**
   - Arduino board
   - LiquidCrystal_I2C library for the LCD display
   - Wire library for I2C communication
   - Piezo buzzer
   - Resistive voltage divider (analogInPin)
   - Relay module
   - Pushbutton
   - Various resistors and wires

**3. Libraries and Definitions:**
   - `Wire.h`: Used for I2C communication.
   - `LiquidCrystal_I2C.h`: Library for interfacing with the I2C LCD display.
   - Constants like `NOTE_B0`, `NOTE_C1`, etc., define musical notes for the buzzer.
   - Global variables to store sensor readings, time, and other parameters.

**4. Buzzer Setup and Melody:**
   - The project begins with an introductory melody played on the buzzer using the `tone` function.
   - Melody is defined using musical notes, and the tempo is set.

**5. LCD Initialization and Display:**
   - The LCD is initialized with the I2C address (0x27), columns (16), and rows (2).
   - Introductory messages are displayed on the LCD during startup.

**6. Main Loop:**
   - A loop reads analog sensor values for voltage and current, processes them, and calculates power, ampere-hours, and energy consumption.
   - The calculated values are then printed to the serial monitor and displayed on the LCD.
   - The relay is controlled based on current limits, and a button press can manually control the relay.

**7. Current Control and Buzzer Feedback:**
   - The `controlRelay` function manages the relay state based on current limits.
   - If the current exceeds 30mA, the relay is turned off, and a message is printed.
   - Button press turns on the relay and triggers the buzzer.

**8. Energy Consumption Display:**
   - The LCD displays real-time values for voltage, current, power, and accumulated energy consumption (ampere-hours and watt-hours).
   - Energy consumption is also converted to units and displayed.

**9. Scrolling Text Functionality:**
   - The `scrollText` function allows scrolling a text message on the LCD during startup.
   - It enhances the user experience by displaying a more extended message in a limited space.

**10. Cost Calculation:**
   - The project includes a basic cost calculation based on the energy consumed and a fixed cost per unit.

**11. Conclusion:**
   - The energy meter provides comprehensive information about electrical parameters.
   - The buzzer and LCD add user-friendly features, making it suitable for practical use.
   - The system has a provision for manual control via a button and automatic current-based relay control.

**12. Further Improvements:**
   - Implement additional safety features or alerts.
   - Optimize the code for better performance.
   - Enhance the user interface with more detailed information on the LCD.

**13. Questions/Clarifications:**
   - The report is based on the provided code. If there are specific functionalities or sections that need more explanation, please specify.

**14. Recommendations:**
   - Consider adding comments to the code for better readability and maintenance.
   - Test the system in various scenarios to ensure accurate readings and reliable performance.

**15. Acknowledgments:**
   - The code includes snippets for musical notes and introductory melody, credited to Robson Couto.

This report provides an overview of the energy meter project, explaining the code's structure, functionality, and potential areas for improvement.
