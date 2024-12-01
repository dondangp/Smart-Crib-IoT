import React, { useState } from "react";
import {
  Text,
  View,
  TouchableOpacity,
  ScrollView,
  Alert,
  TextInput,
} from "react-native";
import { StyleSheet } from "react-native";
import { NavigationContainer } from "@react-navigation/native";
import { createBottomTabNavigator } from "@react-navigation/bottom-tabs";
import { Ionicons } from "@expo/vector-icons";
import axios from "axios";
import styles from "./AppStyles";
import { StatusBar } from "react-native";

// Header Component
const Header = ({ title }) => (
  <View style={styles.header}>
    <Text style={styles.headerText}>{title}</Text>
  </View>
);

// Keypad Component
const Keypad = ({ onPress }) => {
  const keys = [
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "Clear",
    "Enter",
  ];

  return (
    <View style={styles.keypadContainer}>
      {keys.map((key) => (
        <TouchableOpacity
          key={key}
          style={styles.keypadButton}
          onPress={() => onPress(key)}
        >
          <Text style={styles.keypadButtonText}>{key}</Text>
        </TouchableOpacity>
      ))}
    </View>
  );
};

const DoorlockScreen = () => {
  // Function to handle lock/unlock action
  const handleLockAction = async (doorlock) => {
    // `doorlock`: 1 for "lock", 0 for "unlock"
    try {
      const response = await axios.post("http://13.88.157.6:65432", {
        id: 0, // Acting as a controller
        device: 3, // Device is a door lock
        doorlock, // 1 for lock, 0 for unlock
      });

      console.log(`Server response: ${JSON.stringify(response.data)}`);
      Alert.alert(
        "Success",
        doorlock === 1
          ? "Door has been locked successfully!"
          : "Door has been unlocked successfully!"
      );
    } catch (error) {
      console.error(`Error performing lock/unlock:`, error);
      Alert.alert(
        "Error",
        `Failed to ${doorlock === 1 ? "lock" : "unlock"} the door: ${
          error.response?.data?.message || error.message
        }`
      );
    }
  };

  return (
    <View style={styles.container}>
      <Header title="Smart Doorlock" />
      <ScrollView contentContainerStyle={styles.contentContainer}>
        <View style={styles.buttonGroup}>
          {/* Lock Button */}
          <TouchableOpacity
            style={[styles.deviceButton, { backgroundColor: "#4caf50" }]}
            onPress={() => handleLockAction(1)} // Send `doorlock: 1` for lock
          >
            <Text style={styles.deviceButtonText}>Lock</Text>
          </TouchableOpacity>

          {/* Unlock Button */}
          <TouchableOpacity
            style={[styles.deviceButton, { backgroundColor: "#f44336" }]}
            onPress={() => handleLockAction(0)} // Send `doorlock: 0` for unlock
          >
            <Text style={styles.deviceButtonText}>Unlock</Text>
          </TouchableOpacity>
        </View>
      </ScrollView>
    </View>
  );
};

// Lights Screen
const LightsScreen = () => {
  const [brightness, setBrightness] = useState("");

  const adjustBrightness = async () => {
    if (!brightness || isNaN(brightness)) {
      Alert.alert("Error", "Please enter a valid brightness value (0-100).");
      return;
    }

    try {
      const response = await axios.post("http://13.88.157.6:65432", {
        id: 0,
        device: 1,
        brightness: parseInt(brightness, 10),
      });

      console.log(`Server response: ${JSON.stringify(response.data)}`);
      Alert.alert("Success", `Brightness set to: ${brightness}`);
    } catch (error) {
      console.error("Error sending brightness:", error);
      Alert.alert("Error", `Failed to send brightness: ${error.message}`);
    }
  };

  return (
    <View style={styles.container}>
      <Header title="Smart Lights" />
      <ScrollView contentContainerStyle={styles.contentContainer}>
        <TextInput
          style={styles.input}
          placeholder="Enter brightness (0-100)"
          keyboardType="numeric"
          value={brightness}
          onChangeText={setBrightness}
        />
        <TouchableOpacity
          style={styles.deviceButton}
          onPress={adjustBrightness}
        >
          <Text style={styles.deviceButtonText}>Set Brightness</Text>
        </TouchableOpacity>
      </ScrollView>
    </View>
  );
};

// Thermostat Screen
const Smartfan = () => {
  const [fanSpeed, setFanSpeed] = useState(null);

  const adjustFanSpeed = async (speed, label) => {
    try {
      const response = await axios.post("http://13.88.157.6:65432", {
        id: 0,
        device: 2,
        fanSpeed: speed,
      });

      console.log(`Server response: ${JSON.stringify(response.data)}`);
      Alert.alert("Success", `Fan speed set to: ${label}`);
      setFanSpeed(speed); // Update the selected speed
    } catch (error) {
      console.error("Error sending fan speed:", error);
      Alert.alert("Error", `Failed to send fan speed: ${error.message}`);
    }
  };

  return (
    <View style={styles.container}>
      <Header title="Smart Fan" />
      <ScrollView contentContainerStyle={styles.contentContainer}>
        <Text style={styles.title}>Set Fan Speed</Text>
        <View style={styles.buttonGroup}>
          {/* Off Button */}
          <TouchableOpacity
            style={[
              styles.deviceButton,
              fanSpeed === 0 && { backgroundColor: "#f44336" },
            ]}
            onPress={() => adjustFanSpeed(0, "Off")}
          >
            <Text style={styles.deviceButtonText}>Off</Text>
          </TouchableOpacity>

          {/* Medium Button */}
          <TouchableOpacity
            style={[
              styles.deviceButton,
              fanSpeed === 2 && { backgroundColor: "#ffc107" },
            ]}
            onPress={() => adjustFanSpeed(2, "Medium")}
          >
            <Text style={styles.deviceButtonText}>Cool</Text>
          </TouchableOpacity>

          {/* High Button */}
          <TouchableOpacity
            style={[
              styles.deviceButton,
              fanSpeed === 3 && { backgroundColor: "#4caf50" },
            ]}
            onPress={() => adjustFanSpeed(3, "High")}
          >
            <Text style={styles.deviceButtonText}>Hot</Text>
          </TouchableOpacity>
        </View>
      </ScrollView>
    </View>
  );
};
// Settings Screen Component
const SettingsScreen = () => {
  const [newPassword, setNewPassword] = useState("");

  const resetPassword = async () => {
    // Validate input
    if (!newPassword || isNaN(newPassword)) {
      Alert.alert("Error", "Password must be a numeric value.");
      return;
    }

    const passwordInt = parseInt(newPassword, 10); // Convert string to integer

    if (passwordInt < 0 || passwordInt > 9999) {
      Alert.alert(
        "Error",
        "Password must be a numeric value between 0 and 9999."
      );
      return;
    }

    try {
      // Send request to server
      const response = await axios.post("http://13.88.157.6:65432", {
        id: 0, // Controller ID
        device: 31, // Device ID for the door lock
        newPassword: passwordInt, // Ensure password is an integer
      });

      console.log("Server response:", response.data);
      Alert.alert("Success", "Password reset successfully!");
      setNewPassword(""); // Clear input field
    } catch (error) {
      // Handle errors
      console.error("Error resetting password:", error);

      // Extract error message
      const errorMessage =
        error.response?.data?.message ||
        (error.response?.status === 400
          ? "Bad Request: Please check the input."
          : "Network Error: Could not reach the server.");

      Alert.alert("Error", errorMessage);
    }
  };

  return (
    <View style={styles.container}>
      <Header title="Settings" />
      <ScrollView contentContainerStyle={styles.contentContainer}>
        <Text style={styles.title}>Reset Door Lock Password</Text>
        <TextInput
          style={styles.input}
          placeholder="Enter new password (numeric, max 4 digits)"
          value={newPassword}
          onChangeText={(text) => setNewPassword(text)} // Update state
          keyboardType="numeric" // Ensure numeric input
          secureTextEntry // Password input masking
        />
        <TouchableOpacity style={styles.deviceButton} onPress={resetPassword}>
          <Text style={styles.deviceButtonText}>Reset Password</Text>
        </TouchableOpacity>
      </ScrollView>
    </View>
  );
};
// Bottom Tab Navigation with Settings Screen
const Tab = createBottomTabNavigator();

export default function App() {
  return (
    <NavigationContainer>
      <StatusBar style="auto" />
      <Tab.Navigator
        screenOptions={({ route }) => ({
          tabBarIcon: ({ color, size }) => {
            let iconName;
            if (route.name === "Lights") {
              iconName = "bulb-outline";
            } else if (route.name === "Doorlock") {
              iconName = "lock-closed-outline";
            } else if (route.name === "Smart Fan") {
              iconName = "thermometer-outline";
            } else if (route.name === "Settings") {
              iconName = "settings-outline"; // Icon for Settings
            }
            return <Ionicons name={iconName} size={size} color={color} />;
          },
          tabBarActiveTintColor: "#0288d1",
          tabBarInactiveTintColor: "gray",
          tabBarStyle: { backgroundColor: "#f5f5f5" },
        })}
      >
        <Tab.Screen name="Lights" component={LightsScreen} />
        <Tab.Screen name="Doorlock" component={DoorlockScreen} />
        <Tab.Screen name="Smart Fan" component={Smartfan} />
        <Tab.Screen name="Settings" component={SettingsScreen} />
      </Tab.Navigator>
    </NavigationContainer>
  );
}
