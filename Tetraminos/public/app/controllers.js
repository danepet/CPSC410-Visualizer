'use strict';

/* Controllers */

angular.module('myApp.controllers', [])
    .controller('AppCtrl', ['$scope', '$location', '$http', '$routeParams',
        function ($scope, $location, $http, $routeParams) {

            $scope.message = undefined;
            $scope.content = undefined;


            $scope.pongAnalyzer = function () {

                var msg = undefined;
                $http.get('/api/pong')
                    .success(function (data, status, headers, config) {
                        //alert("Start analyzing Mario source code: " + msg.title + " @ " + msg.content);
                        $scope.message = "Start analyzing Pong source code...";
                        $scope.message = data.title;
                        $scope.content = data.content;
                    })
                    .error(function (data, status, headers, config) {
                        alert("Error when sending the AJAX request. ");
                    });


            };

            $scope.racingAnalyzer = function () {

                var msg = undefined;
                $http.get('/api/racing')
                    .success(function (data, status, headers, config) {
                        $scope.message = "Start analyzing Racing source code...";
                        $scope.message = data.title;
                        $scope.content = data.content;
                    })
                    .error(function (data, status, headers, config) {
                        alert("Error when sending the AJAX request. ");
                    });


            };

            $scope.pokemonAnalyzer = function () {

                var msg = undefined;
                $http.get('/api/pokemon')
                    .success(function (data, status, headers, config) {
                        $scope.message = "Start analyzing Pokemon source code...";
                        $scope.message = data.title;
                        $scope.content = data.content;
                    })
                    .error(function (data, status, headers, config) {
                        alert("Error when sending the AJAX request. ");
                    });


            };

            $scope.sudokuAnalyzer = function () {

                var msg = undefined;
                $http.get('/api/sudoku')
                    .success(function (data, status, headers, config) {
                        $scope.message = "Start analyzing Sudoku source code...";
                        $scope.message = data.title;
                        $scope.content = data.content;
                    })
                    .error(function (data, status, headers, config) {
                        alert("Error when sending the AJAX request. ");
                    });


            };

            $scope.pacmanAnalyzer = function () {
                var msg = undefined;
                $http.get('/api/pacman')
                    .success(function (data, status, headers, config) {
                        msg = data;
                        //alert("Start analyzing Pacman source code: " + msg.title + " @ " + msg.content);
                        $scope.message = "Start analyzing Pacman source code...";
                    })
                    .error(function (data, status, headers, config) {
                        alert("Error when sending the AJAX request. ");
                        $scope.message = data;
                    });
                //alert("Start analyzing Pacman source code...");
            };


        }]);








