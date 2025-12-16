def get_zone(dist):
    """
    Determine the color zone based on the measured radar distance.

    Zones are defined for visual display on the GUI (RED, YELLOW, GREEN)
    and indicate proximity to obstacles.

    Args:
        dist (float): Distance measured by the radar (in meters).

    Returns:
        str: Zone identifier for the given distance:
            - "RED_FULL" : Critical distance, closest range
            - "RED"      : Very close range
            - "YELLOW_FULL": Medium range, caution
            - "YELLOW"   : Medium distance
            - "GREEN_FULL": Safe distance
            - "GREEN"    : Slightly further distance
            - "NONE"     : Out of range / no warning
    """
    if dist <= 0.3:
        return "RED_FULL"
    elif dist <= 0.5:
        return "RED"
    elif dist <= 0.7:
        return "YELLOW_FULL"
    elif dist <= 0.9:
        return "YELLOW"
    elif dist <= 1.1:
        return "GREEN_FULL"
    elif dist <= 1.3:
        return "GREEN"
    else:
        return "NONE"
