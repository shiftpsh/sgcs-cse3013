package com.shiftpsh.tetris_ai

data class Result(
    var score: Double,
    var drops: Double,
    var efficiency: Double
) {
    operator fun plus(rhs: Result) = Result(
        score + rhs.score,
        drops + rhs.drops,
        efficiency + rhs.efficiency
    )
    operator fun div(rhs: Double) = Result(
        score / rhs,
        drops / rhs,
        efficiency / rhs
    )
    fun cost(): Double = -(drops + efficiency * 10)
}

fun List<Result>.average(): Result {
    var sum = Result(0.0, 0.0, 0.0)
    val count = size.toDouble()
    forEach { sum += it / count }
    return sum
}