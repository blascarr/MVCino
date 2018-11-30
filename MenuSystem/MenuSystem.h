

#pragma once

#include <inttypes.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#include "MenuConfig.h"

#if I2C
	#include <LiquidCrystal_I2C.h>
	#define DisplayClass LiquidCrystal_I2C
	#warning "Menu: Configured for I2C. Edit 'MenuConfig.h' file to change it."
#else
	#include <LiquidCrystal.h>
	#define DisplayClass LiquidCrystal
	#warning "Menu: Configured for Parallel. Edit 'MenuConfig.h' file to change it."
#endif

/// Data type enum.
/**
Used to store the data type of `void*` so that they can be cast back later.
*/
enum class DataType : uint8_t {
  NOT_USED = 0,
  BOOL = 1, BOOLEAN = 1,
  INT8_T = 8,
  UINT8_T = 9, BYTE = 9,
  INT16_T = 16,
  UINT16_T = 17,
  INT32_T = 32,
  UINT32_T = 33,
  FLOAT = 50, DOUBLE = 50,
  CHAR = 60,
  CHAR_PTR = 61,
  CONST_CHAR_PTR = 62,
  PROG_CONST_CHAR_PTR = 65,
  GLYPH = 70,
};

/// Position enum.
/*
Used to store and set the relative or absolute position of the focus indicator.
*/
enum class Position : uint8_t {
  RIGHT = 1, NORMAL = 1,
  LEFT = 2,
  CUSTOM = 3,
};

#ifndef MENU_H
#define MENU_H


	class RenderLine {
      RenderLine(uint8_t column, uint8_t row)
        : _row(row), _column(column), _focusRow(row - 1),
          _focusColumn(column - 1), _focusPosition(Position::NORMAL),
          _variableCount(0), _focusable(false) {
        for (uint8_t i = 0; i < MAX_VARIABLES; i++) {
          _variable[i] = nullptr;
          _variableType[i] = DataType::NOT_USED;
        }
        for (uint8_t f = 0; f < MAX_FUNCTIONS; f++) {
          _function[f] = 0;
        }
      }

            /// Constructor for one variable/constant.
      /**
      @param column - the column at which the line starts
      @param row - the row at which the line is printed
      @param &variableA - variable/constant to be printed
      */
      template <typename A>
      RenderLine(uint8_t column, uint8_t row, A &variableA)
        : RenderLine(column, row) {
        add_variable(variableA);
      }

      /// Constructor for two variables/constants.
      /**
      @param column - the column at which the line starts
      @param row - the row at which the line is printed
      @param &variableA - variable/constant to be printed
      @param &variableB - variable/constant to be printed
      */
      template <typename A, typename B>
      RenderLine(uint8_t column, uint8_t row,
                 A &variableA, B &variableB)
        : RenderLine(column, row, variableA) {
        add_variable(variableB);
      }

      /// Constructor for three variables/constants.
      /**
      @param column - the column at which the line starts
      @param row - the row at which the line is printed
      @param &variableA - variable/constant to be printed
      @param &variableB - variable/constant to be printed
      @param &variableC - variable/constant to be printed
      */
      template <typename A, typename B, typename C>
      RenderLine(uint8_t column, uint8_t row,
                 A &variableA, B &variableB, C &variableC)
        : RenderLine(column, row, variableA, variableB) {
        add_variable(variableC);
      }

      /// Constructor for four variables/constants.
      /**
      @param column - the column at which the line starts
      @param row - the row at which the line is printed
      @param &variableA - variable/constant to be printed
      @param &variableB - variable/constant to be printed
      @param &variableC - variable/constant to be printed
      @param &variableD - variable/constant to be printed
      */
      template <typename A, typename B, typename C, typename D>
      RenderLine(uint8_t column, uint8_t row,
                 A &variableA, B &variableB, C &variableC, D &variableD)
        : RenderLine(column, row, variableA, variableB, variableC) {
        add_variable(variableD);
      }
	};

	class RenderSystem {
		
		private:
			DisplayClass *_p_liquidCrystal; ///< Pointer to the DisplayClass object

	};

  DataType recognizeType(bool variable);

  /**
  @param variable - variable to be checked
  @returns the data type in `DataType` enum format
  */
  DataType recognizeType(char variable);

  /**
  @param variable - variable to be checked
  @returns the data type in `DataType` enum format
  */
  DataType recognizeType(char* variable);

  /**
  @param variable - variable to be checked
  @returns the data type in `DataType` enum format
  */
  DataType recognizeType(const char* variable);

  /**
  @param variable - variable to be checked
  @returns the data type in `DataType` enum format
  */
  DataType recognizeType(int8_t variable);

  /**
  @param variable - variable to be checked
  @returns the data type in `DataType` enum format
  */
  DataType recognizeType(uint8_t variable);

  /**
  @param variable - variable to be checked
  @returns the data type in `DataType` enum format
  */
  DataType recognizeType(int16_t variable);

  /**
  @param variable - variable to be checked
  @returns the data type in `DataType` enum format
  */
  DataType recognizeType(uint16_t variable);

  /**
  @param variable - variable to be checked
  @returns the data type in `DataType` enum format
  */
  DataType recognizeType(int32_t variable);

  /**
  @param variable - variable to be checked
  @returns the data type in `DataType` enum format
  */
  DataType recognizeType(uint32_t variable);

  /**
  @param variable - variable to be checked
  @returns the data type in `DataType` enum format
  */
  DataType recognizeType(float variable);

  /**
  @param variable - variable to be checked
  @returns the data type in `DataType` enum format
  */
  DataType recognizeType(double variable);
  ///@}


  /// Prints the number passed to it in a specific way.
  /**
  Used for convenience when printing the class's address for indentification.

  @param address - number to be printed
  */
  void print_me(uintptr_t address);

#endif