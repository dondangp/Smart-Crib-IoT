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

// Doorlock Screen with Keypad for Passcode Input
const DoorlockScreen = () => {
  const [passcode, setPasscode] = useState("");

  const handleKeyPress = (key) => {
    if (key === "Clear") {
      setPasscode("");
    } else if (key === "Enter") {
      submitPasscode();
    } else {
      setPasscode((prev) => prev + key);
    }
  };

  const submitPasscode = async () => {
    try {
      const response = await axios.post("http://13.88.157.6:65432", {
        id: 0,
        device: 3,
        passcode: passcode,
      });

      console.log(`Server response: ${JSON.stringify(response.data)}`);
      Alert.alert(
        "Success",
        `Passcode ${response.data.isValid ? "accepted" : "rejected"}`
      );
      setPasscode(""); // Clear passcode after submission
    } catch (error) {
      console.error("Error sending passcode:", error);
      Alert.alert("Error", `Failed to send passcode: ${error.message}`);
    }
  };

  return (
    <View style={styles.container}>
      <Header title="Smart Doorlock" />
      <ScrollView contentContainerStyle={styles.contentContainer}>
        <Text style={styles.title}>Enter Passcode</Text>
        <TextInput
          style={styles.input}
          placeholder="Enter passcode"
          value={passcode}
          secureTextEntry
          editable={false} // Prevent manual input, use keypad
        />
        <Keypad onPress={handleKeyPress} />
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
          placeholder="Enter brightness (0-255)"
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
  const [fanSpeed, setFanSpeed] = useState("");

  const adjustFanSpeed = async () => {
    if (!fanSpeed || isNaN(fanSpeed)) {
      Alert.alert("Error", "Please enter a valid fan speed (0-100).");
      return;
    }

    try {
      const response = await axios.post("http://13.88.157.6:65432", {
        id: 0,
        device: 2,
        fanSpeed: parseInt(fanSpeed, 10),
      });

      console.log(`Server response: ${JSON.stringify(response.data)}`);
      Alert.alert("Success", `Fan speed set to: ${fanSpeed}`);
    } catch (error) {
      console.error("Error sending fan speed:", error);
      Alert.alert("Error", `Failed to send fan speed: ${error.message}`);
    }
  };

  return (
    <View style={styles.container}>
      <Header title="Smart Fan" />
      <ScrollView contentContainerStyle={styles.contentContainer}>
        <TextInput
          style={styles.input}
          placeholder="Enter fan speed (0-100)"
          keyboardType="numeric"
          value={fanSpeed}
          onChangeText={setFanSpeed}
        />
        <TouchableOpacity style={styles.deviceButton} onPress={adjustFanSpeed}>
          <Text style={styles.deviceButtonText}>Set Fan Speed</Text>
        </TouchableOpacity>
      </ScrollView>
    </View>
  );
};

// Bottom Tab Navigation
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
              iconName = "thermometer-outline"; // Updated icon name for Smart Fan
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
      </Tab.Navigator>
    </NavigationContainer>
  );
}
