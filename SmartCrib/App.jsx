import React, { useState } from "react";
import { StatusBar } from "expo-status-bar";
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
import styles from "./AppStyles";
import logo from "./logo.png";

// Header Component
const Header = ({ title }) => (
  <View style={styles.header}>
    <Image source={logo} style={styles.logo} />
    <Text style={styles.headerText}>{title}</Text>
  </View>
);

// Lights Screen
const LightsScreen = () => {
  const [brightness, setBrightness] = useState("");

  const adjustBrightness = () => {
    Alert.alert("Adjusting brightness to: ", brightness);
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
  return (
    <View style={styles.container}>
      <Header title="Smart Doorlock" />
      <ScrollView contentContainerStyle={styles.contentContainer}>
        <Text style={styles.title}>Doorlock Controls Here</Text>
      </ScrollView>
    </View>
  );
};

// Fan Screen
const FanScreen = () => {
  return (
    <View style={styles.container}>
      <Header title="Smart Fan" />
      <ScrollView contentContainerStyle={styles.contentContainer}>
        <Text style={styles.title}>Fan Controls Here</Text>
      </ScrollView>
    </View>
  );
};

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
            } else if (route.name === "Fan") {
              iconName = "aperture-outline";
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
        <Tab.Screen name="Fan" component={FanScreen} />
      </Tab.Navigator>
    </NavigationContainer>
  );
}
