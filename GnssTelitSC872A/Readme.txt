a) GPS satellites are identified by their PRN numbers, which range from 1 to 32.
b) The numbers 33-64 are reserved for WAAS satellites. The WAAS system PRN numbers are
120-138. The offset from NMEA WAAS SV ID to WAAS PRN number is 87. A WAAS PRN
number of 120 minus 87 yields the SV ID of 33. The addition of 87 to the SV ID yields the
WAAS PRN number.
c) The numbers 65-96 are reserved for GLONASS satellites. GLONASS satellites are identified
by 64+satellite slot number. The slot numbers are 1 through 24 for the full GLONASS
constellation of 24 satellites, this gives a range of 65 through 88. The numbers 89 through 96 are
available if slot numbers above 24 are allocated to on-orbit spares.