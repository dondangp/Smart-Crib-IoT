import React, { useState } from "react";
import {
  Text,
  View,
  TouchableOpacity,
  ScrollView,
  Image,
  Alert,
  TextInput,
} from "react-native";
import { NavigationContainer } from "@react-navigation/native";
import { createBottomTabNavigator } from "@react-navigation/bottom-tabs";
import { Ionicons } from "@expo/vector-icons";
import axios from "axios";
import styles from "./AppStyles";
import logo from "./logo.png";
import { StatusBar } from "react-native";

// Header Component
const Header = ({ title }) => (
  <View style={styles.header}>
    <Text style={styles.headerText}>{title}</Text>
  </View>
);

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

// Doorlock Screen
const DoorlockScreen = () => {
  const [lockStatus, setLockStatus] = useState("");

  const toggleLock = async () => {
    if (lockStatus !== "0" && lockStatus !== "1") {
      Alert.alert("Error", "Please enter 0 to unlock or 1 to lock.");
      return;
    }

    try {
      const response = await axios.post("http://13.88.157.6:65432", {
        id: 0,
        device: 3,
        doorlock: parseInt(lockStatus, 10),
      });

      console.log(`Server response: ${JSON.stringify(response.data)}`);
      Alert.alert(
        "Success",
        `Doorlock status set to: ${lockStatus === "1" ? "Locked" : "Unlocked"}`
      );
    } catch (error) {
      console.error("Error sending doorlock status:", error);
      Alert.alert("Error", `Failed to send doorlock status: ${error.message}`);
    }
  };

  return (
    <View style={styles.container}>
      <Header title="Smart Doorlock" />
      <ScrollView contentContainerStyle={styles.contentContainer}>
        <TextInput
          style={styles.input}
          placeholder="Enter 0 to unlock, 1 to lock"
          keyboardType="numeric"
          value={lockStatus}
          onChangeText={setLockStatus}
        />
        <TouchableOpacity style={styles.deviceButton} onPress={toggleLock}>
          <Text style={styles.deviceButtonText}>Set Doorlock Status</Text>
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
