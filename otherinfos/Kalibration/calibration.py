import numpy as np
from scipy import stats

# Die gegebenen Daten
druck_mmHg = [0, 40, 80, 120, 160, 200, 240, 280, 290]
spannung_multimeter = [0.0038, 0.0121, 0.0198, 0.0275, 0.0356, 0.0433, 0.051, 0.0587, 0.0607]
spannung_arduino = [0.0044, 0.014, 0.0231, 0.0324, 0.0417, 0.0503, 0.06, 0.0692, 0.0713]

# Führe die lineare Regression für die Multimeter-Daten durch
slope_multimeter, intercept_multimeter, r_value_multimeter, p_value_multimeter, std_err_multimeter = stats.linregress(druck_mmHg, spannung_multimeter)

# Führe die lineare Regression für die Arduino-Daten durch
slope_arduino, intercept_arduino, r_value_arduino, p_value_arduino, std_err_arduino = stats.linregress(druck_mmHg, spannung_arduino)

# Führe die lineare Regression für die Arduino-Daten für die Spannung aus
v_slope_arduino, v_intercept_arduino, v_r_value_arduino, v_p_value_arduino, v_std_err_arduino = stats.linregress(spannung_multimeter, spannung_arduino)

# Gib die Ergebnisse aus
print("Multimeter: Steigung (Verstärkungsfaktor) =", slope_multimeter)
print("Multimeter: Y-Achsenabschnitt (Offset) =", intercept_multimeter)
print("Multimeter: R-Wert =", r_value_multimeter)

print("Arduino: Steigung (Verstärkungsfaktor) =", slope_arduino)
print("Arduino: Y-Achsenabschnitt (Offset) =", intercept_arduino)
print("Arduino: R-Wert =", r_value_arduino)

print(f"\n\nArduino: Steigung (Verstärkungsfaktor) = {v_slope_arduino}\nArduino: Y-Achsenabschnitt (Offset) = {v_intercept_arduino}\nArduino: R-Wert = {v_r_value_arduino}")